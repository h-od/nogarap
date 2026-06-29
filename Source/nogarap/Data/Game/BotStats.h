#pragma once

#include "BotStats.generated.h"

USTRUCT()
struct NOGARAP_API FBotStats
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Wave;
	UPROPERTY()
	int32 MinionCount;
	UPROPERTY()
	int32 BigMinionCount;
	UPROPERTY()
	int32 MinionHealth;
	UPROPERTY()
	int32 BigMinionHealth;
	UPROPERTY()
	int32 MinionDamage;
	UPROPERTY()
	int32 BigMinionDamage;

	FBotStats() = default;

	FBotStats(
		int32 W,
		int32 MC,
		int32 BMC,
		int32 MH,
		int32 BMH,
		int32 MD,
		int32 BMD
	) : Wave(W), MinionCount(MC), BigMinionCount(BMC), MinionHealth(MH), BigMinionHealth(BMH), MinionDamage(MD), BigMinionDamage(BMD)
	{
	}
};
