#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "nogarap/Data/Character/Characters.h"
#include "MenuCharacter.generated.h"

UCLASS()
class NOGARAP_API AMenuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	ECharacters Character;
	
	virtual void BeginPlay() override;
};
