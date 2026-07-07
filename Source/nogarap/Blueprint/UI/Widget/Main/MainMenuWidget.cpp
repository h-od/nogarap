#pragma once

#include "MainMenuWidget.h"
#include "CoreMinimal.h"
#include "nogarap/Data/Character/Stats/CharacterStats.h"

void UMainMenuWidget::SetHeroStats(const FCharacterStats& CharacterInfo)
{
	//todo save difficulty too
	SetScore(FText::FromString(FString::FromInt(CharacterInfo.TotalScore)));
	SetRed(CharacterInfo.ReadableLevel(CharacterInfo.RedLevel));
	SetBlue(CharacterInfo.ReadableLevel(CharacterInfo.BlueLevel));
	SetGreen(CharacterInfo.ReadableLevel(CharacterInfo.GreenLevel));
	SetAttack(CharacterInfo.ReadableLevel(CharacterInfo.AttackLevel));
	SetDefense(CharacterInfo.ReadableLevel(CharacterInfo.DefenseLevel));
}

void UMainMenuWidget::PreviousDifficulty()
{
	switch (CurrentDifficulty)
	{
	case EDifficulty::Easy:
	default:
		break;
	case EDifficulty::Medium:
		CurrentDifficulty = EDifficulty::Easy;
		SetPreviousDifficultyEnabled(false);
		break;
	case EDifficulty::Hard:
		CurrentDifficulty = EDifficulty::Medium;
		break;
	case EDifficulty::Expert:
		SetNextDifficultyEnabled(true);
		CurrentDifficulty = EDifficulty::Hard;
		break;
	}
	
	SetDifficulty(GetDifficultyText());
}

void UMainMenuWidget::NextDifficulty()
{
	switch (CurrentDifficulty)
	{
	case EDifficulty::Easy:
		CurrentDifficulty = EDifficulty::Medium;
		SetPreviousDifficultyEnabled(true);
		break;
	case EDifficulty::Medium:
		CurrentDifficulty = EDifficulty::Hard;
		break;
	case EDifficulty::Hard:
		CurrentDifficulty = EDifficulty::Expert;
		SetNextDifficultyEnabled(false);
		break;
	case EDifficulty::Expert:
	default:
		break;
	}

	SetDifficulty(GetDifficultyText());
}

FText UMainMenuWidget::GetDifficultyText() const
{
	FText Text;
	switch (CurrentDifficulty)
	{
	case EDifficulty::Easy:
	default:
		Text = FText::FromString("Easy");
		break;
	case EDifficulty::Medium:
		Text = FText::FromString("Medium");
		break;
	case EDifficulty::Hard:
		Text = FText::FromString("Hard");
		break;
	case EDifficulty::Expert:
		Text = FText::FromString("Expert");
		break;
	}
	return Text;
}
