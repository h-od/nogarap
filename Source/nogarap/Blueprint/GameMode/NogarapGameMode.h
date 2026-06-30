#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "nogarap/Data/Character/CharacterGameplay.h"
#include "NogarapGameMode.generated.h"

class UGameCompleteWidget;
class ANogarapCharacter;
class ANogarapController;
class UNogarapGameInstance;
class ASpawnPoint;
struct FBotStats;
class AMinionCharacter;

UCLASS()
class NOGARAP_API ANogarapGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	UNogarapGameInstance* GameInstance;
	UPROPERTY()
	ANogarapController* PlayerController;

	UPROPERTY()
	TMap<int32, ASpawnPoint*> SpawnPoints;
	
	UPROPERTY()
	TArray<FBotStats> MinionStats;
	
	UPROPERTY()
	int32 CurrentWave;
	UPROPERTY()
	int32 MinionsDestroyed;
	UPROPERTY()
	bool bVillainDestroyed = false;
	
	UPROPERTY()
	int32 SpawnPointZeroMinionCount;
	UPROPERTY()
	int32 SpawnPointZeroBigMinionCount;
	
	UPROPERTY()
	int32 SpawnPointOneMinionCount;
	UPROPERTY()
	int32 SpawnPointOneBigMinionCount;
	
	UPROPERTY()
	int32 SpawnPointTwoMinionCount;
	UPROPERTY()
	int32 SpawnPointTwoBigMinionCount;
	
	UPROPERTY()
	int32 SpawnPointThreeMinionCount;
	UPROPERTY()
	int32 SpawnPointThreeBigMinionCount;

	UPROPERTY()
	FTimerHandle SpawnPoint0TimerHandle;
	UPROPERTY()
	FTimerHandle SpawnPoint1TimerHandle;
	UPROPERTY()
	FTimerHandle SpawnPoint2TimerHandle;
	UPROPERTY()
	FTimerHandle SpawnPoint3TimerHandle;
	UPROPERTY()
	FTimerHandle WaveCompleteTimerHandle;
	UPROPERTY()
	FTimerHandle StartWaveTimerHandle;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Config")
	TSubclassOf<AMinionCharacter> MinionClass;
	UPROPERTY(EditDefaultsOnly, Category="Config")
	TSubclassOf<AMinionCharacter> BigMinionClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameCompleteWidget> GameCompleteWidgetClass;

public:
	ANogarapGameMode();

	void MinionDestroyed();
	void VillainDestroyed();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	void GatherSpawnPoints();
	void StartSpawn();
	
	UFUNCTION()
	void SpawnPointZero();
	UFUNCTION()
	void SpawnPointOne();
	UFUNCTION()
	void SpawnPointTwo();
	UFUNCTION()
	void SpawnPointThree();
	UFUNCTION()
	void CheckWaveComplete();
};
