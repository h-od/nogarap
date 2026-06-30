#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCompleteWidget.generated.h"

UCLASS()
class NOGARAP_API UGameCompleteWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainMenu;
	
	UFUNCTION(BlueprintCallable)
	void GoToMainMenu() const;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSummary(const FText& NewScore, const FText& TotalScore);
};
