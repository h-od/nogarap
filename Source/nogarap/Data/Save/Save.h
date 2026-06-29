
#pragma once

#include "CoreMinimal.h"
#include "NogarapSettings.h"
#include "GameFramework/SaveGame.h"
#include "Save.generated.h"

struct FCharacterStats;
enum class ECharacters : uint8;

UCLASS()
class NOGARAP_API USave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FNogarapSettings Settings;
	
	UPROPERTY()
	TMap<ECharacters, FCharacterStats> Characters;
};
