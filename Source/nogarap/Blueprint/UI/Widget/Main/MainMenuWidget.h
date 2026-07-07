#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "nogarap/Data/Character/Difficulty.h"
#include "MainMenuWidget.generated.h"

enum class EDifficulty : uint8;
struct FCharacterStats;

UCLASS()
class NOGARAP_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	EDifficulty CurrentDifficulty = EDifficulty::Easy;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetVillainSelect();

	UFUNCTION()
	void SetHeroStats(const FCharacterStats& CharacterInfo);

	UFUNCTION(BlueprintCallable)
	void PreviousDifficulty();
	UFUNCTION(BlueprintCallable)
	void NextDifficulty();
	UFUNCTION(BlueprintImplementableEvent)
	void SetDifficulty(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetPreviousDifficultyEnabled(bool bEnabled);
	UFUNCTION(BlueprintImplementableEvent)
	void SetNextDifficultyEnabled(bool bEnabled);
	
	FText GetDifficultyText() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetScore(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetRed(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlue(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetGreen(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetAttack(const FText& NewValue);
	UFUNCTION(BlueprintImplementableEvent)
	void SetDefense(const FText& NewValue);
	
	UFUNCTION(BlueprintImplementableEvent)
	void VillainSelected();
	UFUNCTION(BlueprintImplementableEvent)
	void Unfade();
};
