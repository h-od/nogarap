#include "NogarapGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "nogarap/Data/Character/CharacterStats.h"

UNogarapGameInstance::UNogarapGameInstance()
{
}

void UNogarapGameInstance::Load()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName + FString::FromInt(CurrentSaveSlotIndex), CurrentSaveSlotIndex))
	{
		if (const USave* LoadedGame = Cast<USave>(
			UGameplayStatics::LoadGameFromSlot(SaveSlotName + FString::FromInt(CurrentSaveSlotIndex), CurrentSaveSlotIndex)))
		{
			Settings = LoadedGame->Settings;
			CharactersInfo = LoadedGame->Characters;
		}
	}
	else
	{
		Settings.StickSens = .5f;
		Settings.StickSensAiming = .5f;
		Settings.MouseSens = .5f;
		Settings.MouseSensAiming = .5f;

		CharactersInfo = TMap<ECharacters, FCharacterStats>();
		const FCharacterStats NewCharacter = FCharacterStats(
			Score,
			OffensiveTime,
			Offensive,
			MaxOffensive,
			DefensiveTime,
			Defensive,
			MaxDefensive,
			Heal,
			Health,
			MaxHealth,
			Stamina,
			MaxStamina,
			StaminaRegenRate,
			Damage,
			AttackEffort,
			BlockEffort,
			BlockEffect,
			PerfectBlockWindow
		);
		CharactersInfo.Add(ECharacters::Aurora, NewCharacter);
		CharactersInfo.Add(ECharacters::Countess, NewCharacter);
		CharactersInfo.Add(ECharacters::Dekker, NewCharacter);
		CharactersInfo.Add(ECharacters::FengMao, NewCharacter);
		CharactersInfo.Add(ECharacters::Greystone, NewCharacter);
		CharactersInfo.Add(ECharacters::Grux, NewCharacter);
		CharactersInfo.Add(ECharacters::Kallari, NewCharacter);
		CharactersInfo.Add(ECharacters::Khaimera, NewCharacter);
		CharactersInfo.Add(ECharacters::Kwang, NewCharacter);
		CharactersInfo.Add(ECharacters::Morigesh, NewCharacter);
		CharactersInfo.Add(ECharacters::Muriel, NewCharacter);
		CharactersInfo.Add(ECharacters::Revenant, NewCharacter);
		CharactersInfo.Add(ECharacters::Serath, NewCharacter);
		CharactersInfo.Add(ECharacters::Sevarog, NewCharacter);
		CharactersInfo.Add(ECharacters::Shinbi, NewCharacter);
		CharactersInfo.Add(ECharacters::Wukong, NewCharacter);
		CharactersInfo.Add(ECharacters::Terra, NewCharacter);
		CharactersInfo.Add(ECharacters::Yin, NewCharacter);

		Save();
	}
}

void UNogarapGameInstance::OnStart()
{
	Super::OnStart();
	if (UGameplayStatics::DoesSaveGameExist(MetaSlotName, MetaSlotIndex))
	{
		if (const USaveMeta* LoadedGame = Cast<USaveMeta>(
			UGameplayStatics::LoadGameFromSlot(MetaSlotName, MetaSlotIndex)))
		{
			CurrentSaveSlotIndex = LoadedGame->GetMostRecentSlot();
		}
	}

	if (CurrentSaveSlotIndex == 0)
	{
		CurrentSaveSlotIndex = 1;
		if (USaveMeta* SaveGameInstance = Cast<USaveMeta>(
			UGameplayStatics::CreateSaveGameObject(USaveMeta::StaticClass())))
		{
			SaveGameInstance->SlotOne = FDateTime::Now();

			if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, MetaSlotName, MetaSlotIndex))
			{
				UE_LOG(LogTemp, Warning, TEXT("Saved Meta Successfully"));
			}
		}
	}

	Load();
}

void UNogarapGameInstance::UpdateCharacter(const ECharacters Character, const FCharacterStats& CharacterInfo)
{
	CharactersInfo[Character] = CharacterInfo;
	Save();
}

void UNogarapGameInstance::UpdateCurrentCharacter(const FCharacterStats& CharacterInfo)
{
	CharactersInfo[Hero] = CharacterInfo;
	Save();
}

FCharacterStats UNogarapGameInstance::GetStatsForCharacter(const ECharacters Character)
{
	return CharactersInfo[Character];
}

FCharacterStats UNogarapGameInstance::GetStatsForCurrentCharacter()
{
	return CharactersInfo[Hero];
}

void UNogarapGameInstance::SaveCharacterScore(const float NewScore)
{
	CharactersInfo[Hero].Score = NewScore;
	Save();
}

USaveMeta* UNogarapGameInstance::GetSaveMeta() const
{
	if (UGameplayStatics::DoesSaveGameExist("Meta", 0))
	{
		if (USaveMeta* Loaded = Cast<USaveMeta>(
			UGameplayStatics::LoadGameFromSlot(MetaSlotName, MetaSlotIndex)))
		{
			return Loaded;
		}
	}

	return nullptr;
}

int32 UNogarapGameInstance::GetCurrentSaveSlot() const
{
	return CurrentSaveSlotIndex;
}

void UNogarapGameInstance::SetCurrentSaveSlot(const int32 NewCurrent)
{
	CurrentSaveSlotIndex = NewCurrent;
	Load();
}

void UNogarapGameInstance::Save() const
{
	if (USave* SaveGameInstance = Cast<USave>(
		UGameplayStatics::CreateSaveGameObject(USave::StaticClass())))
	{
		SaveGameInstance->Settings = Settings;
		SaveGameInstance->Characters = CharactersInfo;

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName + FString::FromInt(CurrentSaveSlotIndex), CurrentSaveSlotIndex))
		{
			//TODO Feedback?
			UE_LOG(LogTemp, Warning, TEXT("ULttnGameInstance::SaveGame Successfully"));
		}
		else
		{
			// TODO send error to the UI or solve? Why would this ever fail? No free space or something?
			UE_LOG(LogTemp, Warning, TEXT("ULttnGameInstance::SaveGame FAILED"));
		}
	}
	if (USaveMeta* SaveGameInstance = Cast<USaveMeta>(
		UGameplayStatics::LoadGameFromSlot(MetaSlotName, MetaSlotIndex)))
	{
		SaveGameInstance->UpdateSlot(CurrentSaveSlotIndex);

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, MetaSlotName, MetaSlotIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("ULttnGameInstance::SaveGame Successfully"));
		}
	}
}
