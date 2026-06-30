#include "GameOverWidget.h"

#include "Kismet/GameplayStatics.h"

void UGameOverWidget::Reset() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Arena, true, "");
}

void UGameOverWidget::GoToMainMenu() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenu, true, "");
}
