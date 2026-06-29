#include "CharacterGameplay.h"

#include "nogarap/Data/Character/Characters.h"

FCharacterGameplay::FCharacterGameplay() : CurrentCharacter(ECharacters::Aurora), Info(FCharacterStats())
{
}

FCharacterGameplay::FCharacterGameplay(ECharacters Character, const FCharacterStats CharacterInfo) : CurrentCharacter(ECharacters::Aurora)
{
	CurrentCharacter = Character;
	Info = CharacterInfo;
}

void FCharacterGameplay::SetHealthMax()
{
	Info.Health = Info.MaxHealth;
}

int FCharacterGameplay::UpdateHealth(const float Delta)
{
	return Info.Health += Delta;
}

float FCharacterGameplay::GetCurrentHealth() const
{
	// current health (to display)
	return Info.Health / Info.MaxHealth;
}

int32 FCharacterGameplay::UpdateScore(const int32 Delta)
{
	Info.Score += Delta;
	return Info.Score;
}

float FCharacterGameplay::GetScore() const
{
	return Info.Score;
}

float FCharacterGameplay::GetDamage() const
{
	return Info.Damage;
}

float FCharacterGameplay::UpdateStamina(float Delta)
{
	return Info.Stamina += Delta;
}

bool FCharacterGameplay::CanAttack() const
{
	return Info.Stamina >= Info.AttackEffort;
}

float FCharacterGameplay::Attack()
{
	Info.Stamina -= Info.AttackEffort;
	return Info.Stamina;
}

bool FCharacterGameplay::CanBlock() const
{
	return Info.Stamina >= Info.BlockEffort;
}

float FCharacterGameplay::Block()
{
	Info.Stamina -= Info.BlockEffort;
	return Info.Stamina;
}
