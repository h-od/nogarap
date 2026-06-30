#include "NogarapCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "nogarap/Blueprint/Character/Component/GameplayComponent.h"
#include "nogarap/Blueprint/Character/Enemy/EnemyCharacter.h"
#include "nogarap/Blueprint/Controller/NogarapController.h"
#include "nogarap/Blueprint/GameInstance/NogarapGameInstance.h"
#include "nogarap/Blueprint/GameMode/NogarapGameMode.h"

ANogarapCharacter::ANogarapCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GameplayComponent = CreateDefaultSubobject<UGameplayComponent>(TEXT("GameplayComponent"));
}

void ANogarapCharacter::BeginPlay()
{
	Super::BeginPlay();
	UNogarapGameInstance* GameInstance = Cast<UNogarapGameInstance>(GetGameInstance());
	GameplayComponent->SetPlayerManager(
		FCharacterGameplay(
			GameInstance->Hero,
			GameInstance->GetStatsForCurrentCharacter()
		)
	);
}

void ANogarapCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	NogarapController = Cast<ANogarapController>(NewController);
}

void ANogarapCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANogarapCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ANogarapCharacter::MovingStarted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ANogarapCharacter::MovingStopped);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ANogarapCharacter::MovingStopped);
		EnhancedInputComponent->BindAction(Look_MouseAction, ETriggerEvent::Triggered, this, &ANogarapCharacter::Look_MouseTriggered);
		EnhancedInputComponent->BindAction(Look_GamepadAction, ETriggerEvent::Triggered, this, &ANogarapCharacter::Look_GamepadTriggered);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ANogarapCharacter::SprintStarted);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ANogarapCharacter::SprintStopped);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ANogarapCharacter::SprintStopped);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ANogarapCharacter::JumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ANogarapCharacter::JumpStopped);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ANogarapCharacter::JumpStopped);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ANogarapCharacter::AttackStarted);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ANogarapCharacter::BlockStarted);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Canceled, this, &ANogarapCharacter::BlockStopped);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ANogarapCharacter::BlockStopped);
		EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Started, this, &ANogarapCharacter::HealStarted);
		EnhancedInputComponent->BindAction(OffensiveAction, ETriggerEvent::Started, this, &ANogarapCharacter::OffensiveStarted);
		EnhancedInputComponent->BindAction(DefensiveAction, ETriggerEvent::Started, this, &ANogarapCharacter::DefensiveStarted);
	}
}

float ANogarapCharacter::TakeDamage(const float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//TODO when successful block, fill up defensive charge
	if (GameplayComponent->CanPerfectBlock())
	{
		BlockStopped();
		AttackStarted();
		bIsCounterAttacking = true;
		return 0.0f;
	}
	if (!DamageCauser->IsA(StaticClass()))
	{
		Combo = 0;
		NogarapController->SetCombo(Combo);
		if (CurrentAnim)
		{
			StopAnimMontage(CurrentAnim);
		}

		AttackFinished();
		ResetAttack();
		PlayAnimMontage(HitAnim);
		GameplayComponent->TakeDamage(Damage, DamageCauser->GetActorForwardVector());
	}
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ANogarapCharacter::SetStats(const FCharacterGameplay& NewCharacterGameplay)
{
	CharacterGameplay = NewCharacterGameplay;
}

void ANogarapCharacter::SetHealth(const float NewValue) const
{
	NogarapController->SetHealth(NewValue);
}

void ANogarapCharacter::Die() const
{
	GetMesh()->PlayAnimation(DeathAnim, false);
	GetMovementComponent()->Deactivate();
	NogarapController->GameOver();
}

void ANogarapCharacter::SetStamina(const float NewValue) const
{
	NogarapController->SetStamina(NewValue);
}

void ANogarapCharacter::StaminaDepleted()
{
	SprintStopped();
}

void ANogarapCharacter::StopBlocking()
{
	BlockStopped();
}

void ANogarapCharacter::ResetAttack()
{
	AttackCount = 0;
	bSaveAttack = false;
	bIsAttacking = false;
	GameplayComponent->StartStaminaRegen();
}

void ANogarapCharacter::SaveAttack()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		DoAttack();
	}
}

int32 ANogarapCharacter::GetScore() const
{
	return GameplayComponent->GetScore();
}

int32 ANogarapCharacter::GetTotalScore() const
{
	return GameplayComponent->GetTotalScore();
}

void ANogarapCharacter::Hit(AActor* DamagedActor, FVector const& HitFromDirection, FHitResult const& HitInfo, const TSubclassOf<UDamageType> DamageTypeClass) const
{
	if (DamagedActor->IsA(StaticClass()))
	{
		return;
	}

	GameplayComponent->Hit(DamagedActor, HitFromDirection, HitInfo, DamageTypeClass);
}

void ANogarapCharacter::MoveTriggered(const FInputActionValue& InputActionValue)
{
	const float Yaw = GetControlRotation().Yaw;
	const FRotator YawRotation(0, Yaw, 0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(YawRotation);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(YawRotation);
	const FVector2D Input = InputActionValue.Get<FVector2D>();
	const FVector2D ScaleValue = UKismetMathLibrary::Normal2D(Input);

	// UKismetSystemLibrary::PrintString(GetWorld(), "ALttnCharacter::Look_GamepadTriggered X: " + FString::SanitizeFloat(ScaleValue.X) + ", Y: " + FString::SanitizeFloat(ScaleValue.X), true, true, FLinearColor::Red, 5.0f);

	AddMovementInput(RightVector, ScaleValue.X, false);
	AddMovementInput(ForwardVector, ScaleValue.Y, false);
}

void ANogarapCharacter::MovingStarted()
{
	GameplayComponent->SetIsMoving(true);
}

void ANogarapCharacter::MovingStopped()
{
	GameplayComponent->SetIsMoving(false);
}

void ANogarapCharacter::Look_MouseTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxis = InputActionValue.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		AddControllerYawInput(LookAxis.X * MouseSens / 100);
		AddControllerPitchInput(LookAxis.Y * MouseSens / 100);
	}
	else
	{
		UE_LOG(LogHAL, Log, TEXT("ANogarapCharacter::Look_MouseTriggered controller null"));
	}
}

void ANogarapCharacter::Look_GamepadTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxis = InputActionValue.Get<FVector2D>();
	const float WorldDeltaSecs = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	if (GetController() != nullptr)
	{
		AddControllerYawInput(LookAxis.X * (StickSens / 100) * WorldDeltaSecs);
		AddControllerPitchInput(LookAxis.Y * (StickSens / 100) * WorldDeltaSecs);
	}
	else
	{
		UE_LOG(LogHAL, Log, TEXT("ANogarapCharacter::Look_GamepadTriggered controller null"));
	}
}

void ANogarapCharacter::SprintStarted()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	GameplayComponent->StartSprinting();
}

void ANogarapCharacter::SprintStopped()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed; 
	GameplayComponent->StopSprinting();
}

void ANogarapCharacter::JumpStarted()
{
	Jump();
}

void ANogarapCharacter::JumpStopped()
{
	StopJumping();
}

void ANogarapCharacter::AttackStarted()
{
	//TODO when successful attack, fill up offensive charge
	if (bIsAttacking)
	{
		bSaveAttack = true;
		return;
	}
	bIsAttacking = true;
	DoAttack();
}

void ANogarapCharacter::BlockStarted()
{
	if (bIsCounterAttacking)
	{
		return;
	}
	GameplayComponent->SetBlocking(true);
	PlayAnimMontage(BlockAnim);
}

void ANogarapCharacter::BlockStopped()
{
	GameplayComponent->SetBlocking(false);
	StopAnimMontage(BlockAnim);
}

void ANogarapCharacter::DoAttack()
{
	if (!GameplayComponent->CanAttack())
	{
		bSaveAttack = false;
		bIsAttacking = false;
		AttackCount = 0;
		return;
	}
	if (AttackCount >= AttackAnims.Num())
	{
		AttackCount = 0;
	}
	GameplayComponent->Attack();
	UAnimMontage* AnimMontage = AttackAnims[AttackCount];
	CurrentAnim = AnimMontage;
	const float Duration = PlayAnimMontage(AnimMontage);
	StartWeaponTrace(Duration);

	AttackCount++;
	bIsCounterAttacking = false;
}

void ANogarapCharacter::StartWeaponTrace(const float Duration)
{
	WeaponTraceDuration = Duration;
	WeaponTraceInterval = Duration / 100;
	GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
	GetWorldTimerManager().SetTimer(WeaponTraceTimerHandle, this, &ANogarapCharacter::DoWeaponTrace, WeaponTraceInterval, true);
}

void ANogarapCharacter::DoWeaponTrace()
{
	//TODO: damage actor once during an attack, but immediately

	WeaponTraceCurrent += WeaponTraceInterval;
	if (WeaponTraceCurrent >= WeaponTraceDuration)
	{
		AttackFinished();
		return;
	}
	const FVector Start = GetMesh()->GetSocketLocation(WeaponBase);
	const FVector End = GetMesh()->GetSocketLocation(WeaponTip);
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ToIgnore;
	ToIgnore.Add(this);
	FHitResult OutHit;

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 0.1f, TraceObjectTypes, false, ToIgnore, EDrawDebugTrace::Type::None, OutHit, true,
	                                                                    FLinearColor::Red, FLinearColor::Green, 1.0);

	if (const AActor* Actor = OutHit.GetActor(); bHit and Actor and Actor->IsA(AEnemyCharacter::StaticClass()) and !Actor->IsActorBeingDestroyed())
	{
		AttackHits.Add(OutHit);
	}

	if (bIsAkimbo and !bHit)
	{
		const FVector OtherStart = GetMesh()->GetSocketLocation(WeaponBaseOther);
		const FVector OtherEnd = GetMesh()->GetSocketLocation(WeaponTipOther);
		FHitResult OtherOutHit;
		const bool bAnotherHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), OtherStart, OtherEnd, 0.1f, TraceObjectTypes, false, ToIgnore, EDrawDebugTrace::Type::None,
		                                                                           OtherOutHit, true, FLinearColor::Red, FLinearColor::Green, 1.0);
		if (const AActor* Actor = OtherOutHit.GetActor(); bAnotherHit and Actor and Actor->IsA(AEnemyCharacter::StaticClass()) and !Actor->IsActorBeingDestroyed())
		{
			AttackHits.Add(OtherOutHit);
		}
	}

	for (const FHitResult AttackHit : AttackHits)
	{
		if (!HitActor.Contains(AttackHit.GetActor()))
		{
			if (AttackHit.GetActor()->IsA(AEnemyCharacter::StaticClass()))
			{
				Combo++;
				NogarapController->SetCombo(Combo);
				int32 AttackScore = 10;
				if (Combo >= 0)
				{
					AttackScore *= Combo;
				}
				//TODO Hit == 10 points, combo = x1.5, -> over 10 hits combo =x2, 20 hits 2.5 etc
				const int32 NewValue = GameplayComponent->UpdateScore(AttackScore);
				
				
				NogarapController->SetScore(NewValue);
				HitActor.AddUnique(AttackHit.GetActor());
				UGameplayStatics::ApplyPointDamage(AttackHit.GetActor(), GameplayComponent->GetPlayerManager().GetDamage(), AttackHit.ImpactPoint, AttackHit, GetController(), this, nullptr);
			}
		}
	}
}

void ANogarapCharacter::AttackFinished()
{
	CurrentAnim = nullptr;
	GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
	WeaponTraceCurrent = 0;
	WeaponTraceDuration = 0;
	WeaponTraceInterval = 0;


	AttackHits.Empty();
	HitActor.Empty();
}

void ANogarapCharacter::HealStarted()
{
	//TODO max health or add certain amount? maybe last heal skill will be full heal + invincible for 30s
	// also set time 
	PlayAnimMontage(HealAnim);
}

void ANogarapCharacter::OffensiveStarted()
{
	// todo knockback/ damage nearby players and increased damage for X seconds
	PlayAnimMontage(OffensiveAnim);
}

void ANogarapCharacter::DefensiveStarted()
{
	// todo knockback/ stun nearby players and decreased damaged taken for X seconds
	PlayAnimMontage(DefensiveAnim);
}

ANogarapGameMode* ANogarapCharacter::GetGameMode()
{
	if (!NogarapGameMode)
	{
		NogarapGameMode = Cast<ANogarapGameMode>(GetWorld()->GetAuthGameMode());
	}
	return NogarapGameMode;
}
