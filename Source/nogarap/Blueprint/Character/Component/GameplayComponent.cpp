#include "GameplayComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "nogarap/Blueprint/Character/Player/NogarapCharacter.h"
#include "nogarap/Data/Character/CharacterGameplay.h"

UGameplayComponent::UGameplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGameplayComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterStats.SetHealthMax();
	CharacterStats.ResetScore();
}

void UGameplayComponent::SetPlayerManager(const FCharacterGameplay& NewValue)
{
	CharacterStats = NewValue;
}

void UGameplayComponent::TakeDamage(const float DamageToTake, const FVector& DamageDirection)
{
	constexpr float Knockback = 500;

	if (bGreenActive)
	{
		return; //Invincible
	}

	GetCharacter()->LaunchCharacter(FVector(DamageDirection.X, DamageDirection.Y, 0.0f) * Knockback, true, false);

	float Delta = -1 * DamageToTake;
	if (bIsBlocking and CharacterStats.CanBlock())
	{
		Delta /= 2;
		UpdateStamina(CharacterStats.Info.BlockEffort * -1);
		GetCharacter()->SetBlue(CharacterStats.IncrementDefence());
	}
	if (bBlueActive)
	{
		Delta -= CharacterStats.Info.DefensiveEffect; //TODO?
	}
	if (CharacterStats.UpdateHealth(Delta) <= 0)
	{
		GetCharacter()->Die();
	}

	GetCharacter()->SetHealth(CharacterStats.GetCurrentHealth());
}

void UGameplayComponent::SetIsMoving(const bool bMoving)
{
	bIsMoving = bMoving;
	if (bMoving and bIsSprinting)
	{
		StartStaminaDegen();
	}
	else
	{
		StartStaminaRegen();
	}
}

void UGameplayComponent::StartSprinting()
{
	bIsSprinting = true;
	StartStaminaDegen();
}

void UGameplayComponent::StopSprinting()
{
	bIsSprinting = false;
	StartStaminaRegen();
}

void UGameplayComponent::StartStaminaDegen()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StaminaDegenTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(StaminaDegenTimerHandle, this, &UGameplayComponent::DecrementStamina, 0.1f, true);
	StaminaDepletedDelegate.AddDynamic(this, &UGameplayComponent::StopSprinting);
}

void UGameplayComponent::StartStaminaRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StaminaDegenTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UGameplayComponent::IncrementStamina, 0.1f, true);
	StaminaDepletedDelegate.RemoveDynamic(this, &UGameplayComponent::StopSprinting);
}

bool UGameplayComponent::CanAttack() const
{
	return CharacterStats.CanAttack();
}

void UGameplayComponent::Attack()
{
	GetCharacter()->SetStamina(CharacterStats.Attack());
	GetCharacter()->SetRed(CharacterStats.IncrementOffence());
}

void UGameplayComponent::SetBlocking(const bool bBlocking)
{
	bIsBlocking = bBlocking and CharacterStats.CanBlock();
	if (bBlocking)
	{
		StopBlockingDelegate.AddDynamic(this, &UGameplayComponent::StopBlocking);

		bPerfectBlockWindowOpen = true;

		GetWorld()->GetTimerManager().ClearTimer(PerfectBlockTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(PerfectBlockTimerHandle, this, &UGameplayComponent::PerfectBlockWindowClosed, 0.1f, true);
	}
	else
	{
		StopBlockingDelegate.RemoveDynamic(this, &UGameplayComponent::StopBlocking);
	}
}

bool UGameplayComponent::CanPerfectBlock() const
{
	return bIsBlocking and bPerfectBlockWindowOpen; //TODO and can attack?
}

void UGameplayComponent::PerfectBlockWindowClosed()
{
	bPerfectBlockWindowOpen = false;
}

void UGameplayComponent::StopBlocking()
{
	GetCharacter()->StopBlocking();
}

void UGameplayComponent::UpdateStamina(const float Delta)
{
	if (CharacterStats.Info.Stamina > 0)
	{
		if (const float PlayerStamina = CharacterStats.UpdateStamina(Delta); PlayerStamina <= 0)
		{
			GetCharacter()->StaminaDepleted();
			StopSprinting();
		}
		else
		{
			GetCharacter()->SetStamina(PlayerStamina / CharacterStats.Info.MaxStamina);
		}
	}

	if (!CharacterStats.CanBlock())
	{
		StopBlockingDelegate.Broadcast();
	}

	StartStaminaRegen();
}

bool UGameplayComponent::CanHeal() const
{
	return CharacterStats.CanHeal() and !(bBlueActive or bRedActive);
}

void UGameplayComponent::HealStart()
{
	bGreenActive = true;

	//todo, tick it up?
	GetCharacter()->SetHealth(CharacterStats.Heal());

	GetWorld()->GetTimerManager().ClearTimer(AbilityTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AbilityTimerHandle, this, &UGameplayComponent::HealStop, CharacterStats.GetHealTime(), false);
}

void UGameplayComponent::HealStop()
{
	bGreenActive = false;
	GetWorld()->GetTimerManager().ClearTimer(AbilityTimerHandle);
}

bool UGameplayComponent::CanOffensive() const
{
	return CharacterStats.CanOffensive() and !(bGreenActive or bBlueActive);
}

void UGameplayComponent::OffensiveStart()
{
	bRedActive = true;
	GetWorld()->GetTimerManager().ClearTimer(AbilityTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AbilityTimerHandle, this, &UGameplayComponent::OffensiveStop, CharacterStats.GetOffensiveTime(), false);
}

void UGameplayComponent::OffensiveStop()
{
	bRedActive = false;
	GetWorld()->GetTimerManager().ClearTimer(AbilityTimerHandle);
}

bool UGameplayComponent::CanDefensive() const
{
	return CharacterStats.CanDefensive() and !(bGreenActive or bRedActive);
}

void UGameplayComponent::DefensiveStart()
{
	bBlueActive = true;

	GetWorld()->GetTimerManager().ClearTimer(AbilityTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AbilityTimerHandle, this, &UGameplayComponent::DefensiveStop, CharacterStats.GetDefensiveTime(), false);
}

void UGameplayComponent::DefensiveStop()
{
	bBlueActive = false;
	GetWorld()->GetTimerManager().ClearTimer(AbilityTimerHandle);
}

void UGameplayComponent::Hit(AActor* DamagedActor, const FVector& HitFromDirection, const FHitResult& HitInfo, const TSubclassOf<UDamageType> DamageTypeClass)
{
	ANogarapCharacter* DamageCauser = GetCharacter();

	UGameplayStatics::ApplyPointDamage(DamagedActor, CharacterStats.GetDamage(), HitFromDirection, HitInfo, DamageCauser->GetController(), DamageCauser, DamageTypeClass);
}

int32 UGameplayComponent::GetScore() const
{
	return CharacterStats.GetScore();
}

int32 UGameplayComponent::UpdateScore(const int32 Score)
{
	return CharacterStats.UpdateScore(Score);
}

int32 UGameplayComponent::GetTotalScore()
{
	return CharacterStats.GetTotalScore();
}

void UGameplayComponent::PerfectBlock()
{
	GetCharacter()->SetBlue(CharacterStats.IncrementDefence());
	GetCharacter()->SetRed(CharacterStats.IncrementOffence());
}

float UGameplayComponent::GetDamage() const
{
	float Damage = CharacterStats.GetDamage();
	if (bRedActive)
	{
		Damage += (1 * CharacterStats.Info.OffensiveEffect);
	}
	return Damage;
}

ANogarapCharacter* UGameplayComponent::GetCharacter()
{
	if (!Character)
	{
		Character = Cast<ANogarapCharacter>(GetOwner());
	}

	return Character;
}

void UGameplayComponent::DecrementStamina()
{
	if (!bIsMoving)
	{
		return;
	}
	if (CharacterStats.Info.Stamina > 0)
	{
		if (const float PlayerStamina = CharacterStats.UpdateStamina(-1); PlayerStamina <= 0)
		{
			GetCharacter()->StaminaDepleted();
			StopSprinting();
		}
		else
		{
			GetCharacter()->SetStamina(PlayerStamina / CharacterStats.Info.MaxStamina);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaDegenTimerHandle);
		StaminaDepletedDelegate.Broadcast();
	}
}

void UGameplayComponent::IncrementStamina()
{
	if (CharacterStats.Info.Stamina < CharacterStats.Info.MaxStamina)
	{
		GetCharacter()->SetStamina(CharacterStats.UpdateStamina(1) / CharacterStats.Info.MaxStamina);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}
