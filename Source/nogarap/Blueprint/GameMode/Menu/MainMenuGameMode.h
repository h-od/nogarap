#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "nogarap/Blueprint/UI/Widget/Skills/SkillsMenuWidget.h"
#include "MainMenuGameMode.generated.h"

class ULoadMenuWidget;
struct FCharacterStats;
class AVillainCharacter;
class UNogarapGameInstance;
class UMainMenuWidget;
class AMenuCharacter;
class ANogarapCharacter;

UCLASS()
class NOGARAP_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	UNogarapGameInstance* GameInstance;
	
	UPROPERTY()
	AMenuCharacter* CurrentHero;
	UPROPERTY()
	AMenuCharacter* CurrentVillain;
	UPROPERTY()
	int32 HeroIndex;
	UPROPERTY()
	int32 VillainIndex;
	UPROPERTY()
	TArray<TSubclassOf<AMenuCharacter>> HeroKeys;
	UPROPERTY()
	TArray<TSubclassOf<AMenuCharacter>> VillainKeys;

	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;
	UPROPERTY()
	ULoadMenuWidget* LoadMenuWidget;
	UPROPERTY()
	USkillsMenuWidget* SkillsMenuWidget;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadMenuWidget> LoadWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillsMenuWidget> SkillsWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameLevel;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AMenuCharacter>, TSubclassOf<ANogarapCharacter>> Heroes;
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AMenuCharacter>, TSubclassOf<AVillainCharacter>> Villains;

	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable)
	void ShowMainWidget();
	UFUNCTION(BlueprintCallable)
	void ShowLoadWidget();
	UFUNCTION(BlueprintCallable)
	void ShowSkillsWidget();
	UFUNCTION(BlueprintCallable)
	void HideMainWidget() const;
	UFUNCTION(BlueprintCallable)
	void HideLoadWidget() const;
	UFUNCTION(BlueprintCallable)
	void HideSkillsWidget() const;
	
	UFUNCTION()
	FCharacterStats CurrentSavedHeroStats() const;
	UFUNCTION(BlueprintCallable)
	void NextHero();
	UFUNCTION(BlueprintCallable)
	void PreviousHero();
	UFUNCTION(BlueprintCallable)
	void HeroSelected();
	UFUNCTION(BlueprintCallable)
	void ShowVillain();
	
	UFUNCTION(BlueprintCallable)
	void NextVillain();
	UFUNCTION(BlueprintCallable)
	void PreviousVillain();
	UFUNCTION(BlueprintCallable)
	void VillainSelected() const;
	UFUNCTION(BlueprintCallable)
	void MoveVillain() const;
	UFUNCTION(BlueprintCallable)
	void Start() const;

	void SetHeroStats(const FCharacterStats& Info) const;
private:
	void SpawnHero();
	void SpawnVillain();
};
