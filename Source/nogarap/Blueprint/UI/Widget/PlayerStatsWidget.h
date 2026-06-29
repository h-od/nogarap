#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.generated.h"

UCLASS()
class NOGARAP_API UPlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetScore(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetCombo(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealth(float NewValue); // 0 - 1
	UFUNCTION(BlueprintImplementableEvent)
	void SetStamina(float NewValue); // 0 - 1
	UFUNCTION(BlueprintImplementableEvent)
	void SetChargeRed(float NewValue); // 0 - 1
	UFUNCTION(BlueprintImplementableEvent)
	void SetChargeBlue(float NewValue); // 0 - 1
	UFUNCTION(BlueprintImplementableEvent)
	void SetWave(const FText& NewValue);
};
