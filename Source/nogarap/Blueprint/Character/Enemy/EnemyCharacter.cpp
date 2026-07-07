#include "EnemyCharacter.h"

#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "nogarap/Blueprint/Character/Player/NogarapCharacter.h"
#include "nogarap/Blueprint/GameMode/NogarapGameMode.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<ANogarapGameMode>(GetWorld()->GetAuthGameMode());
	SetHealth(1);
}

float AEnemyCharacter::TakeDamage(const float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UpdateHealth(Damage * -1);
	StopAnimMontage();
	PlayAnimMontage(HitAnim);
	
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyCharacter::SetStats(const float NewDamage, const float NewHealth)
{
	MaxHealth = NewHealth;
	Health = NewHealth;
	DealDamage = NewDamage;
}

float AEnemyCharacter::Attack()
{
	bShouldAttackRight = !bShouldAttackRight;
	UAnimMontage* AnimMontage = AttackLeftAnim;
	if (bShouldAttackRight)
	{
		AnimMontage = AttackRightAnim;
	}
	const float Duration = PlayAnimMontage(AnimMontage);
	StartWeaponTrace(Duration);
	return Duration;
}

void AEnemyCharacter::StartStun()
{
	GetCharacterMovement()->StopMovementImmediately();
	PlayAnimMontage(StunnedAnim);
}

void AEnemyCharacter::StopStun()
{
	bIsStunned = false;
	StopAnimMontage(StunnedAnim);
}

void AEnemyCharacter::HitGreen(const FVector& Vector)
{
	StopAnimMontage();
	PlayAnimMontage(HitAnim);
	//todo knockback
	bIsStunned = true;
}

void AEnemyCharacter::HitBlue(const FVector& Vector)
{
	StopAnimMontage();
	//todo knockback
	bIsStunned = true;
}

void AEnemyCharacter::HitRed(const FVector& Vector)
{
	UpdateHealth(0.0f);
}

void AEnemyCharacter::UpdateHealth(const float Delta)
{
	Health += Delta;

	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}

	float NewHealth;
	if (Health <= 0.0f)
	{
		bIsDead = true;
		NewHealth = 0.0f;
		GetMesh()->PlayAnimation(DeadAnim, false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		DelayedDestroy(DeadAnim->GetPlayLength()); //TODO instead of destroy do some sort of dissolving effect
	}
	else
	{
		NewHealth = Health / MaxHealth;
	}
	
	SetHealth(NewHealth);
}

void AEnemyCharacter::DelayedDestroy(const float Duration)
{
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyCharacter::DoDestroy, Duration, false);
}

void AEnemyCharacter::DoDestroy()
{
	Destroy();
}

void AEnemyCharacter::StartWeaponTrace(const float Duration) //TODO this is all duped from char, consider Component
{
	WeaponTraceDuration = Duration;
	WeaponTraceInterval = Duration / 250;
	GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
	GetWorldTimerManager().SetTimer(WeaponTraceTimerHandle, this, &AEnemyCharacter::DoWeaponTrace, WeaponTraceInterval, true);
}

void AEnemyCharacter::DoWeaponTrace()
{
	WeaponTraceCurrent += WeaponTraceInterval;

	if (WeaponTraceCurrent <= WeaponTraceInterval * 50) //wee delay so it doesn't hit on the back swing
	{
		return;
	}

	const bool bHitRight = AttackHitRight.GetActor() != nullptr and AttackHitRight.GetActor()->IsA(ANogarapCharacter::StaticClass());
	const bool bHitLeft = AttackHitLeft.GetActor() != nullptr and AttackHitLeft.GetActor()->IsA(ANogarapCharacter::StaticClass());

	if (WeaponTraceCurrent >= WeaponTraceDuration or bHitRight or bHitLeft)
	{
		AttackFinished();
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ToIgnore;
	ToIgnore.Add(this);

	if (bShouldAttackRight)
	{
		const FVector Start = GetMesh()->GetSocketLocation(WeaponBaseRight);
		const FVector End = GetMesh()->GetSocketLocation(WeaponTipRight);


		FHitResult OutHit;
		const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 0.1f, TraceObjectTypes, false, ToIgnore, EDrawDebugTrace::Type::None, OutHit, true,
		                                                                    FLinearColor::Red, FLinearColor::Green, 1.0);

		if (bHit and OutHit.GetActor()->IsA(ANogarapCharacter::StaticClass()))
		{
			AttackHitRight = OutHit;
			return;
		}
	}

	if (!bShouldAttackRight)
	{
		const FVector OtherStart = GetMesh()->GetSocketLocation(WeaponBaseLeft);
		const FVector OtherEnd = GetMesh()->GetSocketLocation(WeaponTipLeft);
		FHitResult OtherOutHit;
		const bool bAnotherHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), OtherStart, OtherEnd, 0.1f, TraceObjectTypes, false, ToIgnore, EDrawDebugTrace::Type::None,
		                                                                           OtherOutHit, true, FLinearColor::Red, FLinearColor::Green, 1.0);
		if (bAnotherHit and OtherOutHit.GetActor()->IsA(ANogarapCharacter::StaticClass()))
		{
			AttackHitLeft = OtherOutHit;
		}
	}
}

void AEnemyCharacter::AttackFinished()
{
	GetWorldTimerManager().ClearTimer(WeaponTraceTimerHandle);
	WeaponTraceCurrent = 0;
	WeaponTraceDuration = 0;
	WeaponTraceInterval = 0;

	if (AttackHitRight.GetActor() != nullptr and AttackHitRight.GetActor()->IsA(ANogarapCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyPointDamage(AttackHitRight.GetActor(), DealDamage, AttackHitRight.ImpactPoint, AttackHitRight, GetController(), this, nullptr);
	}

	if (AttackHitLeft.GetActor() != nullptr and AttackHitLeft.GetActor()->IsA(ANogarapCharacter::StaticClass()))
	{
		UGameplayStatics::ApplyPointDamage(AttackHitLeft.GetActor(), DealDamage, AttackHitLeft.ImpactPoint, AttackHitLeft, GetController(), this, nullptr);
	}

	AttackHitRight = FHitResult();
	AttackHitLeft = FHitResult();
}
