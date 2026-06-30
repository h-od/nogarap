#include "GameCompleteWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UGameCompleteWidget::GoToMainMenu() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenu, false, "");
}
