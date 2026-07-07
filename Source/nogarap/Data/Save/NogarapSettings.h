
#pragma once

#include "CoreMinimal.h"
#include "nogarap/Data/Character/Difficulty.h"
#include "NogarapSettings.generated.h"

USTRUCT(BlueprintType, Category = "Struct to hold all Gameplay data")
struct NOGARAP_API FNogarapSettings
{
	GENERATED_BODY()

	UPROPERTY()
	float MouseSens;// 0-1
	UPROPERTY()
	float StickSens;// 0-1
	UPROPERTY()
	float MouseSensAiming;// 0-1
	UPROPERTY()
	float StickSensAiming;// 0-1

	FNogarapSettings()
	{
		MouseSens = 0.5f;
		StickSens = 0.5f;
		MouseSensAiming = 0.1f;
		StickSensAiming = 0.1f;
	}
};
