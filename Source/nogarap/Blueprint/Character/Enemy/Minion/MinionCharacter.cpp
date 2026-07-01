#include "MinionCharacter.h"

#include "nogarap/Blueprint/GameMode/NogarapGameMode.h"
#include "nogarap/Blueprint/UI/Widget/EnemyHealthWidget.h"

void AMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMinionCharacter::SetStats(const int32 NewHealth, const int32 NewDamage)
{
	Health = NewHealth;
	MaxHealth = NewHealth;
	DealDamage = NewDamage;
}

void AMinionCharacter::SetHealth(const float NewValue)
{
	if (HealthWidget)
	{
		HealthWidget->SetHealth(Health / MaxHealth);
	}
	if (NewValue == 0 and GameMode)
	{
		GameMode->MinionDestroyed();
	}
}
