#include "NogarapGameMode.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "nogarap/Blueprint/Actor/SpawnPoint.h"
#include "nogarap/Blueprint/Character/Enemy/Minion/MinionCharacter.h"
#include "nogarap/Blueprint/Character/Enemy/Villain/VillainCharacter.h"
#include "nogarap/Blueprint/Character/Player/NogarapCharacter.h"
#include "nogarap/Blueprint/Controller/NogarapController.h"
#include "nogarap/Blueprint/GameInstance/NogarapGameInstance.h"
#include "nogarap/Blueprint/UI/Widget/GameComplete/GameCompleteWidget.h"
#include "nogarap/Data/Game/BotStats.h"

ANogarapGameMode::ANogarapGameMode()
{
	MinionStats.Add(FBotStats(0, 3, 0, 5, 0, 1, 0));
	MinionStats.Add(FBotStats(1, 5, 1, 5, 10, 1, 2));
	MinionStats.Add(FBotStats(2, 5, 3, 5, 10, 1, 2));
	MinionStats.Add(FBotStats(3, 3, 10, 5, 10, 1, 2));
	MinionStats.Add(FBotStats(4, 3, 5, 5, 10, 1, 2));
}

void ANogarapGameMode::MinionDestroyed()
{
	MinionsDestroyed++;
}

void ANogarapGameMode::VillainDestroyed()
{
	bVillainDestroyed = true;
}

void ANogarapGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ANogarapGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GameInstance = Cast<UNogarapGameInstance>(GetGameInstance());

	GatherSpawnPoints();

	ANogarapCharacter* Character = GetWorld()->SpawnActor<ANogarapCharacter>(GameInstance->HeroToSpawn, SpawnPoints[-1]->GetSpawnPoint());

	PlayerController = Cast<ANogarapController>(NewPlayer);
	if (Character)
	{
		Character->SetStats(
			FCharacterGameplay(
				GameInstance->Hero,
				GameInstance->GetStatsForCurrentCharacter()
			)
		);
		PlayerController->Possess(Character);
	}
	CurrentWave = 0;
	GetWorldTimerManager().ClearTimer(StartWaveTimerHandle);
	GetWorldTimerManager().SetTimer(StartWaveTimerHandle, this, &ANogarapGameMode::StartSpawn, 5.0f, false);
}

void ANogarapGameMode::GatherSpawnPoints()
{
	TArray<AActor*> FoundSpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), FoundSpawnPoints);

	for (AActor* FoundSpawnPoint : FoundSpawnPoints)
	{
		if (FoundSpawnPoint->IsA(ASpawnPoint::StaticClass()))
		{
			ASpawnPoint* Spawn = Cast<ASpawnPoint>(FoundSpawnPoint);
			SpawnPoints.Add(Spawn->SpawnIndex, Spawn);
		}
	}
}

void ANogarapGameMode::StartSpawn()
{
	//TODO: maybe limit how many can spawn to 1 big and 3 small per spawner
	PlayerController->SetWave(CurrentWave);
	GetWorldTimerManager().ClearTimer(StartWaveTimerHandle);

	GetWorldTimerManager().ClearTimer(SpawnPoint0TimerHandle);
	GetWorldTimerManager().SetTimer(SpawnPoint0TimerHandle, this, &ANogarapGameMode::SpawnPointZero, 2.5f, true);
	GetWorldTimerManager().ClearTimer(SpawnPoint1TimerHandle);
	GetWorldTimerManager().SetTimer(SpawnPoint1TimerHandle, this, &ANogarapGameMode::SpawnPointOne, 2.5f, true);
	GetWorldTimerManager().ClearTimer(SpawnPoint2TimerHandle);
	GetWorldTimerManager().SetTimer(SpawnPoint2TimerHandle, this, &ANogarapGameMode::SpawnPointTwo, 2.5f, true);
	GetWorldTimerManager().ClearTimer(SpawnPoint3TimerHandle);
	GetWorldTimerManager().SetTimer(SpawnPoint3TimerHandle, this, &ANogarapGameMode::SpawnPointThree, 2.5f, true);

	GetWorldTimerManager().ClearTimer(WaveCompleteTimerHandle);
	GetWorldTimerManager().SetTimer(WaveCompleteTimerHandle, this, &ANogarapGameMode::CheckWaveComplete, 5.0f, true);
}

void ANogarapGameMode::SpawnPointZero()
{
	//TODO this spawn logic is horrific, need to optimize code and should probably pool these but w/e just getting it working for now
	//small minions
	if (SpawnPointZeroMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		//TODO set damage and health
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(MinionClass, SpawnPoints[0]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].MinionHealth, MinionStats[CurrentWave].MinionDamage);
			SpawnPointZeroMinionCount++;
		}
	}

	//big minions
	if (SpawnPointZeroBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(BigMinionClass, SpawnPoints[0]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].BigMinionHealth, MinionStats[CurrentWave].BigMinionDamage);
			SpawnPointZeroBigMinionCount++;
		}
	}

	if (SpawnPointZeroMinionCount == MinionStats[CurrentWave].MinionCount and SpawnPointZeroBigMinionCount == MinionStats[CurrentWave].BigMinionCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnPoint0TimerHandle);
	}
}

void ANogarapGameMode::SpawnPointOne()
{
	//small minions
	if (SpawnPointOneMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(MinionClass, SpawnPoints[1]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].MinionHealth, MinionStats[CurrentWave].MinionDamage);
			SpawnPointOneMinionCount++;
		}
	}

	//big minions
	if (SpawnPointOneBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(BigMinionClass, SpawnPoints[1]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].BigMinionHealth, MinionStats[CurrentWave].BigMinionDamage);
			SpawnPointOneBigMinionCount++;
		}
	}

	if (SpawnPointOneMinionCount == MinionStats[CurrentWave].MinionCount and SpawnPointOneBigMinionCount == MinionStats[CurrentWave].BigMinionCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnPoint1TimerHandle);
	}
}

void ANogarapGameMode::SpawnPointTwo()
{
	//small minions
	if (SpawnPointTwoMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(MinionClass, SpawnPoints[2]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].MinionHealth, MinionStats[CurrentWave].MinionDamage);
			SpawnPointTwoMinionCount++;
		}
	}

	//big minions
	if (SpawnPointTwoBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(BigMinionClass, SpawnPoints[2]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].BigMinionHealth, MinionStats[CurrentWave].BigMinionDamage);
			SpawnPointTwoBigMinionCount++;
		}
	}

	if (SpawnPointTwoMinionCount == MinionStats[CurrentWave].MinionCount and SpawnPointTwoBigMinionCount == MinionStats[CurrentWave].BigMinionCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnPoint2TimerHandle);
	}
}

void ANogarapGameMode::SpawnPointThree()
{
	//small minions
	if (SpawnPointThreeMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(MinionClass, SpawnPoints[3]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].MinionHealth, MinionStats[CurrentWave].MinionDamage);
			SpawnPointThreeMinionCount++;
		}
	}

	//big minions
	if (SpawnPointThreeBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(BigMinionClass, SpawnPoints[3]->GetSpawnPoint()))
		{
			MinionCharacter->SetStats(MinionStats[CurrentWave].BigMinionHealth, MinionStats[CurrentWave].BigMinionDamage);
			SpawnPointThreeBigMinionCount++;
		}
	}

	if (SpawnPointThreeMinionCount == MinionStats[CurrentWave].MinionCount and SpawnPointThreeBigMinionCount == MinionStats[CurrentWave].BigMinionCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnPoint3TimerHandle);
	}
}

void ANogarapGameMode::CheckWaveComplete()
{
	const int32 SpawnedMinionsCount = SpawnPointZeroMinionCount + SpawnPointZeroBigMinionCount +
		SpawnPointOneMinionCount + SpawnPointOneBigMinionCount +
		SpawnPointTwoMinionCount + SpawnPointTwoBigMinionCount +
		SpawnPointThreeMinionCount + SpawnPointThreeBigMinionCount;
	const int32 MinionsThatShouldHaveSpawned = (MinionStats[CurrentWave].BigMinionCount + MinionStats[CurrentWave].MinionCount) * 4;

	if (MinionsDestroyed == SpawnedMinionsCount and MinionsDestroyed == MinionsThatShouldHaveSpawned)
	{
		GetWorldTimerManager().ClearTimer(WaveCompleteTimerHandle);

		CurrentWave++;
		if (CurrentWave == 4)
		{
			FActorSpawnParameters SpawnParams = FActorSpawnParameters();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AVillainCharacter* VillainCharacter = GetWorld()->SpawnActor<AVillainCharacter>(GameInstance->VillainToSpawn, SpawnPoints[-2]->GetSpawnPoint(), SpawnParams);

			// spawn villain todo set stats
		}

		MinionsDestroyed = 0;
		SpawnPointZeroMinionCount = 0;
		SpawnPointZeroBigMinionCount = 0;
		SpawnPointOneMinionCount = 0;
		SpawnPointOneBigMinionCount = 0;
		SpawnPointTwoMinionCount = 0;
		SpawnPointTwoBigMinionCount = 0;
		SpawnPointThreeMinionCount = 0;
		SpawnPointThreeBigMinionCount = 0;


		if (CurrentWave < 5)
		{
			//start timer to start next wave
			GetWorldTimerManager().ClearTimer(StartWaveTimerHandle);
			GetWorldTimerManager().SetTimer(StartWaveTimerHandle, this, &ANogarapGameMode::StartSpawn, 5.0f, false);
		}
		else
		{
			UGameCompleteWidget* UserWidget = Cast<UGameCompleteWidget>(CreateWidget(GetWorld(), GameCompleteWidgetClass));
			const ANogarapCharacter* NogarapCharacter = Cast<ANogarapCharacter>(PlayerController->GetCharacter());
			const int32 NewScore = NogarapCharacter->GetScore();
			const int32 TotalScore = NogarapCharacter->GetTotalScore();
			UserWidget->ShowSummary(FText::FromString(FString::FromInt(NewScore)), FText::FromString(FString::FromInt(TotalScore)));
			UserWidget->AddToViewport();

			GameInstance->SaveCharacterScore(TotalScore);
		}
	}
}
