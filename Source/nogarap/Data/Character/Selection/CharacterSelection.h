#pragma once

#include "CoreMinimal.h"
#include "nogarap/Data/Character/Stats/CharacterStats.h"
#include "Templates/SubclassOf.h"
#include "CharacterSelection.generated.h"

class ACharacter;
class ANogarapCharacter;
class AVillainCharacter;
class AMenuCharacter;
enum class ECharacters : uint8;

USTRUCT()
struct NOGARAP_API FChars
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ECharacters Character;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMenuCharacter> Menu;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> Game;
};

USTRUCT()
struct NOGARAP_API FCharacterSelection
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<int32, FChars> IndexedHeroes;
	UPROPERTY()
	TMap<int32, FChars> IndexedVillains;
	
	UPROPERTY()
	int32 CurrentHeroIndex;
	UPROPERTY()
	int32 CurrentVillainIndex;
	
	UPROPERTY()
	FChars CurrentHero;
	UPROPERTY()
	FChars CurrentVillain;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FChars> Heroes;
	UPROPERTY(EditDefaultsOnly)
	TArray<FChars> Villains;
	
public:
	FCharacterSelection();
	
	void SetHero(const ECharacters Character);
	
	TSubclassOf<AMenuCharacter> MenuHero();
	TSubclassOf<AMenuCharacter> MenuVillain();
	
	TSubclassOf<AMenuCharacter> NextHero();
	TSubclassOf<AMenuCharacter> PreviousHero();
	TSubclassOf<AMenuCharacter> NextVillain();
	TSubclassOf<AMenuCharacter> PreviousVillain();
	
	TSubclassOf<ACharacter> Hero();
	TSubclassOf<ACharacter> Villain() const;
};
