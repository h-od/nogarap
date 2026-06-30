#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

UCLASS()
class NOGARAP_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainMenu;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Arena;
	
	UFUNCTION(BlueprintCallable)
	void Reset() const;

	UFUNCTION(BlueprintCallable)
	void GoToMainMenu() const;
};
