#pragma once

#include "SkillsMenuWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "nogarap/Blueprint/GameInstance/NogarapGameInstance.h"
#include "nogarap/Blueprint/GameMode/Menu/MainMenuGameMode.h"
#include "nogarap/Data/Character/CharacterStats.h"

void USkillsMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameInstance = Cast<UNogarapGameInstance>(GetGameInstance());
}

void USkillsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetHeroStats(GameMode->CurrentSavedHeroStats());
}

void USkillsMenuWidget::SetHeroStats(const FCharacterStats& CharacterInfo)
{
	Info = CharacterInfo;
	SetScore(CharacterInfo.IntToText(CharacterInfo.Score));
	SetRed(
		CharacterInfo.ReadableLevel(CharacterInfo.RedLevel),
		CharacterInfo.CanUpgradeRed(),
		CharacterInfo.IsMaxLevel(CharacterInfo.RedLevel),
		CharacterInfo.IntToText(CharacterInfo.RedUpgradeCost())
	);
	SetBlue(
		CharacterInfo.ReadableLevel(CharacterInfo.BlueLevel),
		CharacterInfo.CanUpgradeBlue(),
		CharacterInfo.IsMaxLevel(CharacterInfo.BlueLevel),
		CharacterInfo.IntToText(CharacterInfo.BlueUpgradeCost())
	);
	SetGreen(
		CharacterInfo.ReadableLevel(CharacterInfo.GreenLevel),
		CharacterInfo.CanUpgradeGreen(),
		CharacterInfo.IsMaxLevel(CharacterInfo.GreenLevel),
		CharacterInfo.IntToText(CharacterInfo.GreenUpgradeCost())
	);
	SetAttack(
		CharacterInfo.ReadableLevel(CharacterInfo.AttackLevel),
		CharacterInfo.CanUpgradeAttack(),
		CharacterInfo.IsMaxLevel(CharacterInfo.AttackLevel),
		CharacterInfo.IntToText(CharacterInfo.AttackUpgradeCost())
	);
	SetDefense(
		CharacterInfo.ReadableLevel(CharacterInfo.DefenseLevel),
		CharacterInfo.CanUpgradeDefense(),
		CharacterInfo.IsMaxLevel(CharacterInfo.DefenseLevel),
		CharacterInfo.IntToText(CharacterInfo.DefenseUpgradeCost())
	);
}

void USkillsMenuWidget::UpgradeRed()
{
	//TODO all this should be encapsulated somewhere else preferably
	// MaxOffensive, OffensiveDamage, OffensiveEffect, OffensiveTime
	if (Info.RedLevel >= 9)
	{
		return;
	}
	Info.RedLevel++;
	Info.Score -= Info.GetCostForLevel(Info.RedLevel);
	Info.MaxOffensive -= 10.0;
	Info.OffensiveDamage *= 1.5;
	Info.OffensiveEffect *= 1.5;
	Info.OffensiveTime *= 1.5;
	SetHeroStats(Info);
	GameInstance->UpdateCurrentCharacter(Info);
	GameMode->SetHeroStats(Info);
}

void USkillsMenuWidget::UpgradeBlue()
{
	//TODO MaxDefensive, DefensiveDamage, OffensiveEffect, DefensiveTime
	if (Info.BlueLevel >= 9)
	{
		return;
	}
	Info.BlueLevel++;
	Info.Score -= Info.GetCostForLevel(Info.BlueLevel);
	Info.MaxDefensive -= 10.0;
	Info.DefensiveDamage *= 1.5;
	Info.DefensiveEffect *= 1.5;
	Info.DefensiveTime *= 1.5;
	SetHeroStats(Info);
	GameInstance->UpdateCurrentCharacter(Info);
	GameMode->SetHeroStats(Info);
}

void USkillsMenuWidget::UpgradeGreen()
{
	//TODO Heal, MaxHealth, MaxStamina, StaminaRegenRate
	if (Info.GreenLevel >= 9)
	{
		return;
	}
	Info.GreenLevel++;
	Info.Score -= Info.GetCostForLevel(Info.GreenLevel);
	Info.Heal += 0.1f;
	Info.MaxHealth *= 1.5f;
	Info.MaxStamina *= 1.5f;
	Info.StaminaRegenRate -= 0.05f; // todo?
	SetHeroStats(Info);
	GameInstance->UpdateCurrentCharacter(Info);
	GameMode->SetHeroStats(Info);
}

void USkillsMenuWidget::UpgradeAttack()
{
	//TODO Damage, Attack Effort
	if (Info.AttackLevel >= 9)
	{
		return;
	}
	Info.AttackLevel++;
	Info.Score -= Info.GetCostForLevel(Info.AttackLevel);
	Info.Damage += Info.AttackLevel;
	Info.AttackEffort /= 1.5;
	SetHeroStats(Info);
	GameInstance->UpdateCurrentCharacter(Info);
	GameMode->SetHeroStats(Info);
}

void USkillsMenuWidget::UpgradeDefense()
{
	//TODO BlockEffort, BlockEffect, PerfectBlockWindow-> 0.1 first
	if (Info.DefenseLevel >= 9)
	{
		return;
	}
	Info.DefenseLevel++;
	Info.Score -= Info.GetCostForLevel(Info.DefenseLevel);
	Info.BlockEffort -= Info.GetCostForLevel(Info.DefenseLevel);
	Info.BlockEffect += Info.AttackLevel;
	Info.PerfectBlockWindow += 0.2f;
	SetHeroStats(Info);
	GameInstance->UpdateCurrentCharacter(Info);
	GameMode->SetHeroStats(Info);
}

void USkillsMenuWidget::Back() const
{
	GameMode->HideSkillsWidget();
	GameMode->ShowMainWidget();
}
