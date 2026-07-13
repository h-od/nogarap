#include "VillainCharacter.h"

#include "nogarap/Blueprint/GameMode/NogarapGameMode.h"
#include "nogarap/Blueprint/UI/Widget/BossHealthWidget.h"

AVillainCharacter::AVillainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVillainCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetBossWidget();
}

void AVillainCharacter::SetHealth(const float NewValue)
{
	GetBossWidget()->SetHealth(NewValue);

	if (NewValue == 0 and GameMode)
	{
		GameMode->VillainDestroyed();
	}
}

UBossHealthWidget* AVillainCharacter::GetBossWidget()
{
	if (!HealthWidget)
	{
		HealthWidget = Cast<UBossHealthWidget>(CreateWidget(GetWorld(), HealthWidgetClass));
		HealthWidget->AddToViewport();
	}

	return HealthWidget;
}
