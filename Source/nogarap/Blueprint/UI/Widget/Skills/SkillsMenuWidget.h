#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "nogarap/Data/Character/CharacterStats.h"
#include "SkillsMenuWidget.generated.h"

class UNogarapGameInstance;
class AMainMenuGameMode;
struct FCharacterStats;

UCLASS()
class NOGARAP_API USkillsMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	FCharacterStats Info;

protected:
	UPROPERTY()
	AMainMenuGameMode* GameMode;
	UPROPERTY()
	UNogarapGameInstance* GameInstance;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetHeroStats(const FCharacterStats& CharacterInfo);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetScore(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetRed(const FText& NewValue, const bool bCanUpgrade, const bool bIsMaxLevel, const FText& UpgradeCost);
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlue(const FText& NewValue, const bool bCanUpgrade, const bool bIsMaxLevel, const FText& UpgradeCost);
	UFUNCTION(BlueprintImplementableEvent)
	void SetGreen(const FText& NewValue, const bool bCanUpgrade, const bool bIsMaxLevel, const FText& UpgradeCost);
	UFUNCTION(BlueprintImplementableEvent)
	void SetAttack(const FText& NewValue, const bool bCanUpgrade, const bool bIsMaxLevel, const FText& UpgradeCost);
	UFUNCTION(BlueprintImplementableEvent)
	void SetDefense(const FText& NewValue, const bool bCanUpgrade, const bool bIsMaxLevel, const FText& UpgradeCost);

	UFUNCTION(BlueprintCallable)
	void UpgradeRed();
	UFUNCTION(BlueprintCallable)
	void UpgradeBlue();
	UFUNCTION(BlueprintCallable)
	void UpgradeGreen();
	UFUNCTION(BlueprintCallable)
	void UpgradeAttack();
	UFUNCTION(BlueprintCallable)
	void UpgradeDefense();
	UFUNCTION(BlueprintCallable)
	void Back() const;
};
