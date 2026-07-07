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

	FBotStats() = default;

	FBotStats(
		int32 W,
		int32 MC,
		int32 BMC
	) : Wave(W), MinionCount(MC), BigMinionCount(BMC)
	{
	}
};
