#include "CharacterGameplay.h"

#include "Kismet/KismetSystemLibrary.h"
#include "nogarap/Data/Character/Characters.h"
#include "nogarap/Data/Character/Difficulty.h"
#include "nogarap/Data/Character/Stats/CharacterStats.h"

FCharacterGameplay::FCharacterGameplay() : CurrentCharacter(ECharacters::Aurora), Info(FCharacterStats())
{
}

FCharacterGameplay::FCharacterGameplay(ECharacters Character, const FCharacterStats& CharacterInfo) : CurrentCharacter(ECharacters::Aurora)
{
	CurrentCharacter = Character;
	Info = CharacterInfo;
}

void FCharacterGameplay::SetHealthMax()
{
	Info.Health = Info.MaxHealth;
}

void FCharacterGameplay::ResetScore()
{
	Info.Score = 0;
}

int FCharacterGameplay::UpdateHealth(const float Delta)
{
	if (const float X = Info.Health += Delta; X >= Info.MaxHealth)
	{
		Info.Health = Info.MaxHealth;
	}
	return Info.Health;
}

float FCharacterGameplay::GetCurrentHealth() const
{
	// current health (to display)
	return Info.Health / Info.MaxHealth;
}

int32 FCharacterGameplay::UpdateScore(const int32 Delta)
{
	Info.Score += (Delta * GetDifficultyMultiplier());
	return Info.Score;
}

float FCharacterGameplay::GetDifficultyMultiplier() const
{
	float Multiplier;
	switch (Info.Difficulty)
	{
	case EDifficulty::Easy:
		Multiplier = 1.0f;
		break;
	case EDifficulty::Medium:
		Multiplier = 1.5f;
		break;
	case EDifficulty::Hard:
		Multiplier = 2.0f;
		break;
	case EDifficulty::Expert:
		Multiplier = 2.5f;
		break;
	}


	return Multiplier;
}

float FCharacterGameplay::GetScore() const
{
	return Info.Score;
}

int32 FCharacterGameplay::GetTotalScore()
{
	return Info.TotalScore += Info.Score;
}

float FCharacterGameplay::GetDamage() const
{
	return Info.Damage;
}

float FCharacterGameplay::UpdateStamina(const float Delta)
{
	if (const float X = Info.Stamina += Delta; X >= Info.MaxStamina)
	{
		Info.Stamina = Info.MaxStamina;
	}
	return Info.Stamina;
}

bool FCharacterGameplay::CanAttack() const
{
	return Info.Stamina >= Info.AttackEffort;
}

float FCharacterGameplay::Attack()
{
	Info.Stamina -= Info.AttackEffort;
	return Info.Stamina / Info.MaxStamina;
}

bool FCharacterGameplay::CanBlock() const
{
	return Info.Stamina >= Info.BlockEffort;
}

float FCharacterGameplay::Block()
{
	Info.Stamina -= Info.BlockEffort;
	return Info.Stamina / Info.MaxStamina;
}

bool FCharacterGameplay::CanHeal() const
{
	return CanOffensive() and CanDefensive();
}

bool FCharacterGameplay::CanOffensive() const
{
	return Info.Offensive == Info.MaxOffensive;
}

bool FCharacterGameplay::CanDefensive() const
{
	return Info.Defensive == Info.MaxDefensive;
}

float FCharacterGameplay::GetHealTime() const
{
	return Info.HealTime;
}

float FCharacterGameplay::GetOffensiveTime() const
{
	return Info.OffensiveTime;
}

float FCharacterGameplay::GetDefensiveTime() const
{
	return Info.DefensiveTime;
}

float FCharacterGameplay::Heal()
{
	if (const float NewHealth = Info.Health + (Info.Heal * Info.MaxHealth); NewHealth > Info.MaxHealth)
	{
		Info.Health = Info.MaxHealth;
	}
	else
	{
		Info.Health = NewHealth;
	}

	Offensive();
	Defensive();

	return Info.Health;
}

void FCharacterGameplay::Offensive()
{
	Info.Offensive = 0.0f;
}

void FCharacterGameplay::Defensive()
{
	Info.Defensive = 0.0f;
}

float FCharacterGameplay::IncrementOffence()
{
	if (const float X = Info.Offensive += Info.OffensiveIncrement; X >= Info.MaxOffensive)
	{
		Info.Offensive = Info.MaxOffensive;
	}
	return Info.Offensive / Info.MaxOffensive;
}

float FCharacterGameplay::IncrementDefence()
{
	if (const float X = Info.Defensive += Info.DefensiveIncrement; X >= Info.MaxDefensive)
	{
		Info.Defensive = Info.MaxDefensive;
	}
	return Info.Defensive / Info.MaxDefensive;
}
