#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class NOGARAP_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetVillainSelect();

	UFUNCTION()
	void SetHeroStats(const FCharacterStats& CharacterInfo);

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

inline void UMainMenuWidget::SetHeroStats(const FCharacterStats& CharacterInfo)
{
	SetScore(FText::FromString(FString::FromInt(CharacterInfo.Score)));
	SetRed(CharacterInfo.ReadableLevel(CharacterInfo.RedLevel));
	SetBlue(CharacterInfo.ReadableLevel(CharacterInfo.BlueLevel));
	SetGreen(CharacterInfo.ReadableLevel(CharacterInfo.GreenLevel));
	SetAttack(CharacterInfo.ReadableLevel(CharacterInfo.AttackLevel));
	SetDefense(CharacterInfo.ReadableLevel(CharacterInfo.DefenseLevel));
}
