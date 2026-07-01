#pragma once

#include "CoreMinimal.h"
#include "CharacterStats.h"
#include "CharacterGameplay.generated.h"

enum class ECharacters : uint8;

USTRUCT()
struct NOGARAP_API FCharacterGameplay
{
	GENERATED_BODY()

	FCharacterGameplay();
	explicit FCharacterGameplay(const ECharacters Character, const FCharacterStats Info);

	UPROPERTY()
	ECharacters CurrentCharacter;

	UPROPERTY()
	FCharacterStats Info;

	void SetHealthMax();
	void ResetScore();
	int UpdateHealth(float Delta);
	float GetCurrentHealth() const;
	int32 UpdateScore(int32 Delta);
	float GetScore() const;
	int32 GetTotalScore();
	float GetDamage() const;
	float UpdateStamina(float Delta);

	bool CanAttack() const;
	float Attack();
	bool CanBlock() const;
	float Block();
	
	bool CanHeal() const;
	bool CanOffensive() const;
	bool CanDefensive() const;
	float GetHealTime() const;
	float GetOffensiveTime() const;
	float GetDefensiveTime() const;
	
	float Heal();
	void Offensive();
	void Defensive();
	float IncrementOffence();
	float IncrementDefence();
};
