#pragma once

#include <string>

#include "CoreMinimal.h"
#include "EnemyStats.generated.h"

USTRUCT()
struct NOGARAP_API FEnemyStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Health;
	UPROPERTY(EditDefaultsOnly)
	float Damage;

	FEnemyStats() = default;

	FEnemyStats(
		const float NewHealth,
		const float NewDamage
	) 
	{
		Health = NewHealth;
		Damage = NewDamage;
	}

	FString ToString() const
	{
		const FString HealthString = FString("Health: ") + FString(std::to_string(Health).c_str());
		const FString DamageString = FString("Damage: ") + FString(std::to_string(Damage).c_str());

		return "\t" + HealthString + "\n"
			+ "\t" + DamageString + "\n";
	}
};
