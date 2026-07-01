#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveMeta.generated.h"

struct FCharacterStats;
enum class ECharacters : uint8;

UCLASS()
class NOGARAP_API USaveMeta : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FDateTime SlotOne;
	UPROPERTY()
	FDateTime SlotTwo;
	UPROPERTY()
	FDateTime SlotThree;
	UPROPERTY()
	ECharacters SlotOneCharacter;
	UPROPERTY()
	ECharacters SlotTwoCharacter;
	UPROPERTY()
	ECharacters SlotThreeCharacter;

	int32 GetMostRecentSlot() const;
	ECharacters GetCharacterForSlot(int32 Slot) const;
	void SetCharacterForSlot(int32 Slot, ECharacters Char);
	void UpdateSlot(int32 Slot);
};

inline int32 USaveMeta::GetMostRecentSlot() const
{
	TMap<int32, FDateTime> Slots;
	Slots.Add(1, SlotOne);
	Slots.Add(2, SlotTwo);
	Slots.Add(3, SlotThree);
	int32 MostRecentSlot = 0;
	int64 MostRecent = 0;

	for (TTuple Slot : Slots)
	{
		if (const int64 Timestamp = Slot.Value.ToUnixTimestamp(); Timestamp > MostRecent)
		{
			MostRecentSlot = Slot.Key;
			MostRecent = Timestamp;
		}
	}

	return MostRecentSlot;
}

inline ECharacters USaveMeta::GetCharacterForSlot(const int32 Slot) const
{
	switch (Slot)
	{
	case 1: return SlotOneCharacter;
	case 2: return SlotTwoCharacter;
	case 3: return SlotThreeCharacter;
	default: return ECharacters::Aurora;
	}
}

inline void USaveMeta::SetCharacterForSlot(const int32 Slot, const ECharacters Char)
{
	switch (Slot)
	{
	case 1: SlotOneCharacter = Char;
		return;
	case 2: SlotTwoCharacter = Char;
		return;
	case 3: SlotThreeCharacter = Char;
		return;
	default: ;//TODO?
	}
}

inline void USaveMeta::UpdateSlot(const int32 Slot)
{
	switch (Slot)
	{
	case 1:
		{
			SlotOne = FDateTime::Now();
			break;
		}
	case 2:
		{
			SlotTwo = FDateTime::Now();
			break;
		}
	case 3:
		{
			SlotThree = FDateTime::Now();
			break;
		}
	default: ;
	}
}
