#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class NOGARAP_API ASpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly)
	int32 SpawnIndex;
	
	ASpawnPoint();
	
	UFUNCTION(BlueprintNativeEvent)
	FTransform GetSpawnPoint();
};
