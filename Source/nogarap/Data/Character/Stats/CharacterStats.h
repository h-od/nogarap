#pragma once

#include <string>

#include "CoreMinimal.h"
#include "CharacterStats.generated.h"

enum class EDifficulty : uint8;

USTRUCT()
struct NOGARAP_API FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Score;
	UPROPERTY()
	int32 TotalScore;
	UPROPERTY()
	EDifficulty Difficulty;

	UPROPERTY()
	int32 RedLevel = 0;
	UPROPERTY()
	int32 BlueLevel = 0;
	UPROPERTY()
	int32 GreenLevel = 0;
	UPROPERTY()
	int32 AttackLevel = 0;
	UPROPERTY()
	int32 DefenseLevel = 0;

	//RED
	UPROPERTY()
	float OffensiveTime;
	UPROPERTY()
	float Offensive;
	UPROPERTY()
	float OffensiveIncrement;
	UPROPERTY()
	float MaxOffensive;
	UPROPERTY()
	float OffensiveEffect;
	//BLUE
	UPROPERTY()
	float DefensiveTime;
	UPROPERTY()
	float Defensive;
	UPROPERTY()
	float DefensiveIncrement;
	UPROPERTY()
	float MaxDefensive;
	UPROPERTY()
	float DefensiveEffect; //0-1  
 	//GREEN
	UPROPERTY()
	float Heal; // 10%, 20, 30, 40 etc
	UPROPERTY()
	float HealTime;
	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;
	UPROPERTY()
	float Stamina;
	UPROPERTY()
	float MaxStamina;
	UPROPERTY()
	float StaminaRegenRate;
	//ATTACK
	UPROPERTY()
	float Damage;
	UPROPERTY()
	float AttackEffort;
	//DEFENSE
	UPROPERTY()
	float BlockEffort;
	UPROPERTY()
	float BlockEffect; // when not perfect block how much health is lost IncomingDamage*0.75 to start  
	UPROPERTY()
	float PerfectBlockWindow;

	FCharacterStats() = default;

	FCharacterStats(
		const float NewScore,
		const EDifficulty NewDifficulty,
		const float NewOffensiveTime,
		const float NewOffensive,
		const float NewMaxOffensive,
		const float NewMaxOffensiveIncrement,
		const float NewOffensiveEffect,
		const float NewDefensiveTime,
		const float NewDefensive,
		const float NewDefensiveIncrement,
		const float NewMaxDefensive,
		const float NewDefensiveEffect,
		const float NewHeal,
		const float NewHealTime,
		const float NewHealth,
		const float NewMaxHealth,
		const float NewStamina,
		const float NewMaxStamina,
		const float NewStaminaRegenRate,
		const float NewDamage,
		const float NewAttackEffort,
		const float NewBlockEffort,
		const float NewBlockEffect,
		const float NewPerfectBlockWindow
	) 
	{
		TotalScore = NewScore;
		Score = 0;
		Difficulty = NewDifficulty;
		OffensiveTime = NewOffensiveTime;
		Offensive = NewOffensive;
		OffensiveIncrement = NewMaxOffensiveIncrement;
		OffensiveEffect = NewOffensiveEffect;
		MaxOffensive = NewMaxOffensive;
		DefensiveTime = NewDefensiveTime;
		Defensive = NewDefensive;
		DefensiveIncrement = NewDefensiveIncrement;
		MaxDefensive = NewMaxDefensive;
		DefensiveEffect = NewDefensiveEffect;
		Heal = NewHeal;
		HealTime = NewHealTime;
		Health = NewHealth;
		MaxHealth = NewMaxHealth;
		Stamina = NewStamina;
		MaxStamina = NewMaxStamina;
		StaminaRegenRate = NewStaminaRegenRate;
		Damage = NewDamage;
		AttackEffort = NewAttackEffort;
		BlockEffort = NewBlockEffort;
		BlockEffect = NewBlockEffect;
		PerfectBlockWindow = NewPerfectBlockWindow;
	}

	FString ToString() const
	{
		const FString HealthString = FString("Health: ") + FString(std::to_string(Health).c_str());
		const FString MaxHealthString = FString("MaxHealth: ") + FString(std::to_string(MaxHealth).c_str());
		const FString DamageString = FString("Damage: ") + FString(std::to_string(Damage).c_str());
		const FString AttackEffortString = FString("AttackEffort: ") + FString(std::to_string(AttackEffort).c_str());
		const FString BlockEffortString = FString("BlockEffort: ") + FString(std::to_string(BlockEffort).c_str());
		const FString BlockEffectString = FString("BlockEffect: ") + FString(std::to_string(BlockEffect).c_str());
		const FString PerfectBlockWindowString = FString("PerfectBlockWindow: ") + FString(std::to_string(PerfectBlockWindow).c_str());
		const FString StaminaString = FString("Stamina: ") + FString(std::to_string(Stamina).c_str());
		const FString MaxStaminaString = FString("MaxStamina: ") + FString(std::to_string(MaxStamina).c_str());
		const FString StaminaRegenRateString = FString("StaminaRegenRate: ") + FString(
			std::to_string(StaminaRegenRate).c_str());
		const FString OffensiveString = FString("Offensive: ") + FString(std::to_string(Offensive).c_str());
		const FString MaxOffensiveString = FString("MaxOffensive: ") + FString(std::to_string(MaxOffensive).c_str());
		const FString DefensiveString = FString("Defensive: ") + FString(std::to_string(Defensive).c_str());
		const FString MaxDefensiveString = FString("MaxDefensive: ") + FString(std::to_string(MaxDefensive).c_str());

		return "\t" + HealthString + "\n"
			+ "\t" + MaxHealthString + "\n"
			+ "\t" + DamageString + "\n"
			+ "\t" + AttackEffortString + "\n"
			+ "\t" + BlockEffortString + "\n"
			+ "\t" + BlockEffectString + "\n"
			+ "\t" + PerfectBlockWindowString + "\n"
			+ "\t" + StaminaString + "\n"
			+ "\t" + MaxStaminaString + "\n"
			+ "\t" + StaminaRegenRateString + "\n"
			+ "\t" + OffensiveString + "\n"
			+ "\t" + MaxOffensiveString + "\n"
			+ "\t" + DefensiveString + "\n"
			+ "\t" + MaxDefensiveString + "\n";
	}

	bool CanUpgradeRed() const;
	bool CanUpgradeBlue() const;
	bool CanUpgradeGreen() const;
	bool CanUpgradeAttack() const;
	bool CanUpgradeDefense() const;
	
	int32 RedUpgradeCost() const;
	int32 BlueUpgradeCost() const;
	int32 GreenUpgradeCost() const;
	int32 AttackUpgradeCost() const;
	int32 DefenseUpgradeCost() const;
	
	int32 GetCostForLevel(int32 Level) const;

	static FText ReadableLevel(const int32 Level);
	static bool IsMaxLevel(int32 Level);
	static FText IntToText(int32 Int);
private:
	int32 UpgradeBaseCost = 1000;
	int32 MaxLevel = 10;
};
