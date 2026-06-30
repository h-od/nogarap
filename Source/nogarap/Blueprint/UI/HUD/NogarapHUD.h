#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NogarapHUD.generated.h"

class UGameOverWidget;
class UPlayerStatsWidget;
class UBossHealthWidget;

UCLASS()
class NOGARAP_API ANogarapHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY()
	UPlayerStatsWidget* PlayerStatsWidget;
	UPROPERTY()
	UGameOverWidget* GameSummaryWidget;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerStatsWidget> PlayerStatsWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameOverWidget> GameSummaryWidgetClass;

	virtual void BeginPlay() override;

public:
	void SetScore(int32 NewValue);
	void SetCombo(int32 NewValue);

	void SetPlayerHealth(float NewValue);
	void SetPlayerStamina(float NewValue);
	void SetChargeRed(float NewValue);
	void SetChargeBlue(float NewValue);
	void SetWave(int32 NewValue);
	void ShowGameSummary();

	UPlayerStatsWidget* GetPlayerWidget();
};
