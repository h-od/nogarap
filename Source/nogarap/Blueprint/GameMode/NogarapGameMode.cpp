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
	MinionStats.Add(FBotStats(0, 3, 0));
	MinionStats.Add(FBotStats(1, 5, 1));
	MinionStats.Add(FBotStats(2, 5, 3));
	MinionStats.Add(FBotStats(3, 3, 1));
	MinionStats.Add(FBotStats(4, 3, 5));
}

void ANogarapGameMode::MinionDestroyed(const int32 SpawnIndex)
{
	switch (SpawnIndex)
	{
	case 0:
		SpawnPoint0Destroyed++;
		break;
	case 1:
		SpawnPoint1Destroyed++;
		break;
	case 2:
		SpawnPoint2Destroyed++;
		break;
	case 3:
		SpawnPoint3Destroyed++;
		break;
	default: ; //TODO panic
	}
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
		const FCharacterStats Info = GameInstance->GetStatsForCurrentCharacter();
		Difficulty = Info.Difficulty;
		Character->SetStats(
			FCharacterGameplay(
				GameInstance->Hero,
				Info
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
	PlayerController->SetWave(CurrentWave);
	GetWorldTimerManager().ClearTimer(StartWaveTimerHandle);

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ANogarapGameMode::Spawn, 1.0f, true);

	GetWorldTimerManager().ClearTimer(WaveCompleteTimerHandle);
	GetWorldTimerManager().SetTimer(WaveCompleteTimerHandle, this, &ANogarapGameMode::CheckWaveComplete, 5.0f, true);
}

void ANogarapGameMode::Spawn()
{
	constexpr int32 BotSpawnCap = 1;

	if (SpawnPointZeroCount - SpawnPoint0Destroyed < BotSpawnCap)
	{
		SpawnPointZero();
	}
	if (SpawnPointOneCount - SpawnPoint1Destroyed < BotSpawnCap)
	{
		SpawnPointOne();
	}
	if (SpawnPointTwoCount - SpawnPoint2Destroyed < BotSpawnCap)
	{
		SpawnPointTwo();
	}
	if (SpawnPointThreeCount - SpawnPoint3Destroyed < BotSpawnCap)
	{
		SpawnPointThree();
	}
}

void ANogarapGameMode::SpawnPointZero()
{
	//TODO this better. should probably pool these but w/e just getting it working for now
	//small minions
	if (SpawnPointZeroMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		if (SpawnWeeMinion(0))
		{
			SpawnPointZeroMinionCount++;
			SpawnPointZeroCount++;
		}
		return;
	}

	//big minions
	if (SpawnPointZeroBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (SpawnBigMinion(0))
		{
			SpawnPointZeroBigMinionCount++;
			SpawnPointZeroCount++;
		}
	}
}

void ANogarapGameMode::SpawnPointOne()
{
	//small minions
	if (SpawnPointOneMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		if (SpawnWeeMinion(1))
		{
			SpawnPointOneMinionCount++;
			SpawnPointOneCount++;
		}
		return;
	}

	//big minions
	if (SpawnPointOneBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (SpawnBigMinion(1))
		{
			SpawnPointOneBigMinionCount++;
			SpawnPointOneCount++;
		}
	}
}

void ANogarapGameMode::SpawnPointTwo()
{
	//small minions
	if (SpawnPointTwoMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		if (SpawnWeeMinion(2))
		{
			SpawnPointTwoMinionCount++;
			SpawnPointTwoCount++;
		}
		return;
	}

	//big minions
	if (SpawnPointTwoBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (SpawnBigMinion(2))
		{
			SpawnPointTwoBigMinionCount++;
			SpawnPointTwoCount++;
		}
	}
}

void ANogarapGameMode::SpawnPointThree()
{
	//small minions
	if (SpawnPointThreeMinionCount < MinionStats[CurrentWave].MinionCount)
	{
		if (SpawnWeeMinion(3))
		{
			SpawnPointThreeMinionCount++;
			SpawnPointThreeCount++;
		}
		return;
	}

	//big minions
	if (SpawnPointThreeBigMinionCount < MinionStats[CurrentWave].BigMinionCount)
	{
		if (SpawnBigMinion(3))
		{
			SpawnPointThreeBigMinionCount++;
			SpawnPointThreeCount++;
		}
	}
}

bool ANogarapGameMode::SpawnWeeMinion(const int32 SpawnIndex)
{
	if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(MinionClass, SpawnPoints[SpawnIndex]->GetSpawnPoint()))
	{
		const float DifficultyModifier = GetDifficultyModifier();
		MinionCharacter->SetStats(WeeMinion.Damage * DifficultyModifier, WeeMinion.Health * DifficultyModifier);
		MinionCharacter->SpawnPoint = SpawnIndex;
		return true;
	}
	return false;
}

bool ANogarapGameMode::SpawnBigMinion(const int32 SpawnIndex)
{
	if (AMinionCharacter* MinionCharacter = GetWorld()->SpawnActor<AMinionCharacter>(BigMinionClass, SpawnPoints[SpawnIndex]->GetSpawnPoint()))
	{
		const float DifficultyModifier = GetDifficultyModifier();
		MinionCharacter->SetStats(BigMinion.Damage * DifficultyModifier, BigMinion.Health * DifficultyModifier);
		MinionCharacter->SpawnPoint = SpawnIndex;
		return true;
	}
	return false;
}

void ANogarapGameMode::CheckWaveComplete()
{
	const int32 SpawnedMinionsCount = SpawnPointZeroMinionCount + SpawnPointZeroBigMinionCount +
		SpawnPointOneMinionCount + SpawnPointOneBigMinionCount +
		SpawnPointTwoMinionCount + SpawnPointTwoBigMinionCount +
		SpawnPointThreeMinionCount + SpawnPointThreeBigMinionCount;
	const int32 MinionsThatShouldHaveSpawned = (MinionStats[CurrentWave].BigMinionCount + MinionStats[CurrentWave].MinionCount) * 4;

	if (SpawnedMinionsCount == MinionsThatShouldHaveSpawned)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
	else
	{
		// if timer stopped for whatever reason, restart it
		if (!GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
		{
			GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ANogarapGameMode::Spawn, 1.0f, true);
		}
	}
	if (MinionsDestroyed == MinionsThatShouldHaveSpawned)
	{
		GetWorldTimerManager().ClearTimer(WaveCompleteTimerHandle);

		CurrentWave++;
		if (CurrentWave == 4)
		{
			FActorSpawnParameters SpawnParams = FActorSpawnParameters();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AVillainCharacter* VillainCharacter = GetWorld()->SpawnActor<AVillainCharacter>(GameInstance->VillainToSpawn, SpawnPoints[-2]->GetSpawnPoint(), SpawnParams);
			const float DifficultyModifier = GetDifficultyModifier();
			VillainCharacter->SetStats(Villain.Damage * DifficultyModifier, Villain.Health * DifficultyModifier);
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

float ANogarapGameMode::GetDifficultyModifier() const
{
	float Modifier = 1.0f;
	switch (Difficulty)
	{
	case EDifficulty::Easy:
		Modifier = 1.0f;
		break;
	case EDifficulty::Medium:
		Modifier = 2.0f;
		break;
	case EDifficulty::Hard:
		Modifier = 3.0f;
		break;
	case EDifficulty::Expert:
		Modifier = 5.0f;
		break;
	}
	return Modifier;
}
