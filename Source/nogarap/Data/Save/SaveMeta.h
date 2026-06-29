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

	int32 GetMostRecentSlot() const;
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
