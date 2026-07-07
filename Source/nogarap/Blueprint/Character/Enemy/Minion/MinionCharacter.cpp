#include "MinionCharacter.h"

#include "nogarap/Blueprint/GameMode/NogarapGameMode.h"
#include "nogarap/Blueprint/UI/Widget/EnemyHealthWidget.h"

void AMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMinionCharacter::SetHealth(const float NewValue)
{
	if (HealthWidget)
	{
		HealthWidget->SetHealth(NewValue);
	}
	if (NewValue == 0 and GameMode)
	{
		GameMode->MinionDestroyed(SpawnPoint);
	}
}
