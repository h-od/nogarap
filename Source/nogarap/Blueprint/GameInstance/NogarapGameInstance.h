#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "nogarap/Blueprint/Character/Menu/MenuCharacter.h"
#include "nogarap/Data/Character/Stats/CharacterStats.h"
#include "nogarap/Data/Save/Save.h"
#include "nogarap/Data/Save/SaveMeta.h"
#include "NogarapGameInstance.generated.h"

class AVillainCharacter;
class ANogarapCharacter;

UCLASS()
class NOGARAP_API UNogarapGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY()
	int32 MetaSlotIndex = 0;
	UPROPERTY()
	FString MetaSlotName = "Meta";

	UPROPERTY()
	ECharacters CurrentCharacter = ECharacters::Aurora;
	UPROPERTY()
	int32 CurrentSaveSlotIndex = 0;
	UPROPERTY()
	FString SaveSlotName = "PlayerConfig";

	UPROPERTY()
	FNogarapSettings Settings;
	UPROPERTY()
	TMap<ECharacters, FCharacterStats> CharactersInfo;

protected:
	//FOR DEBUG
	UPROPERTY(EditDefaultsOnly)
	float TotalScore;
	//RED
	UPROPERTY(EditDefaultsOnly)
	float OffensiveTime;
	UPROPERTY(EditDefaultsOnly)
	float Offensive;
	UPROPERTY(EditDefaultsOnly)
	float OffensiveIncrement;
	UPROPERTY(EditDefaultsOnly)
	float MaxOffensive;
	UPROPERTY(EditDefaultsOnly)
	float OffensiveEffect;
	//BLUE
	UPROPERTY(EditDefaultsOnly)
	float DefensiveTime;
	UPROPERTY(EditDefaultsOnly)
	float Defensive;
	UPROPERTY(EditDefaultsOnly)
	float DefensiveIncrement;
	UPROPERTY(EditDefaultsOnly)
	float MaxDefensive;
	UPROPERTY(EditDefaultsOnly)
	float DefensiveEffect;
	//GREEN
	UPROPERTY(EditDefaultsOnly)
	float Heal;// 10%, 20, 30, 40 etc
	UPROPERTY(EditDefaultsOnly)
	float HealTime; 
	UPROPERTY(EditDefaultsOnly)
	float Health;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly)
	float Stamina;
	UPROPERTY(EditDefaultsOnly)
	float MaxStamina;
	UPROPERTY(EditDefaultsOnly)
	float StaminaRegenRate;
	//ATTACK
	UPROPERTY(EditDefaultsOnly)
	float Damage;
	UPROPERTY(EditDefaultsOnly)
	float AttackEffort;
	//DEFENSE
	UPROPERTY(EditDefaultsOnly)
	float BlockEffort;
	UPROPERTY(EditDefaultsOnly)
	float BlockEffect; // when not perfect block how much health is lost IncomingDamage*0.75 to start  
	UPROPERTY(EditDefaultsOnly)
	float PerfectBlockWindow;
	//FOR DEBUG END
public:
	UPROPERTY()
	ECharacters Hero = ECharacters::Aurora;
	UPROPERTY()
	TSubclassOf<ANogarapCharacter> HeroToSpawn;
	UPROPERTY()
	TSubclassOf<AVillainCharacter> VillainToSpawn;

	UNogarapGameInstance();
	
	void Load();

protected:
	virtual void OnStart() override;

public:
	void UpdateCharacter(ECharacters Character, const FCharacterStats& CharacterInfo);
	void UpdateCurrentCharacter(const FCharacterStats& CharacterInfo);
	FCharacterStats GetStatsForCharacter(ECharacters Character);
	FCharacterStats GetStatsForCurrentCharacter();
	void SaveCharacterScore(const float NewScore);
	USaveMeta* GetSaveMeta() const;
	int32 GetCurrentSaveSlot() const;
	void SetCurrentCharacter(ECharacters Character);
	void SetCurrentSaveSlot(int32 NewCurrent);

private:
	void Save() const;
};
