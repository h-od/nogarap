#pragma once

#include "CharacterStats.h"
#include "CoreMinimal.h"

bool FCharacterStats::CanUpgradeRed() const
{
	return RedLevel < MaxLevel and GetCostForLevel(RedLevel + 1) <= Score;
}

bool FCharacterStats::CanUpgradeBlue() const
{
	return BlueLevel < MaxLevel and GetCostForLevel(BlueLevel + 1) <= Score;
}

bool FCharacterStats::CanUpgradeGreen() const
{
	return GreenLevel < MaxLevel and GetCostForLevel(GreenLevel + 1) <= Score;
}

bool FCharacterStats::CanUpgradeAttack() const
{
	return AttackLevel < MaxLevel and GetCostForLevel(AttackLevel + 1) <= Score;
}

bool FCharacterStats::CanUpgradeDefense() const
{
	return DefenseLevel < MaxLevel and GetCostForLevel(DefenseLevel + 1) <= Score;
}

int32 FCharacterStats::RedUpgradeCost() const
{
	return GetCostForLevel(RedLevel + 1);
}

int32 FCharacterStats::BlueUpgradeCost() const
{
	return GetCostForLevel(BlueLevel + 1);
}

int32 FCharacterStats::GreenUpgradeCost() const
{
	return GetCostForLevel(GreenLevel + 1);
}

int32 FCharacterStats::AttackUpgradeCost() const
{
	return GetCostForLevel(AttackLevel + 1);
}

int32 FCharacterStats::DefenseUpgradeCost() const
{
	return GetCostForLevel(DefenseLevel + 1);
}

int32 FCharacterStats::GetCostForLevel(const int32 Level) const
{
	return UpgradeBaseCost * Level;
}

FText FCharacterStats::ReadableLevel(const int32 Level)
{
	if (IsMaxLevel(Level))
	{
		return FText::FromString("Max");
	}
	return FText::FromString(FString::FromInt(Level + 1));
}

bool FCharacterStats::IsMaxLevel(const int32 Level)
{
	return Level == 9;
}

FText FCharacterStats::IntToText(const int32 Int)
{
	return FText::FromString(FString::FromInt(Int));
}