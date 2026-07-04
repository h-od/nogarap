#pragma once

#include "CharacterSelection.h"
#include "CoreMinimal.h"
#include "nogarap/Data/Character/Characters.h"

class ACharacter;

FCharacterSelection::FCharacterSelection() : CurrentHeroIndex(0), CurrentVillainIndex(0), CurrentHero(ECharacters::Aurora), CurrentVillain(ECharacters::Aurora)
{
}

void FCharacterSelection::SetHero(const ECharacters Character)
{
	int32 Index = 0;
	for (const FChars Hero : Heroes)
	{
		IndexedHeroes.Add(Index, Hero);
		if (Hero.Character == Character)
		{
			CurrentHero = Hero;
			CurrentHeroIndex = Index;
		}

		Index++;
	}

	Index = 0;

	for (const FChars Villain : Villains)
	{
		IndexedVillains.Add(Index, Villain);
		if (Index == 0)
		{
			CurrentVillain = Villain;
			CurrentVillainIndex = Index;
		}
		Index++;
	}
}

TSubclassOf<AMenuCharacter> FCharacterSelection::MenuHero()
{
	return CurrentHero.Menu;
}

TSubclassOf<AMenuCharacter> FCharacterSelection::MenuVillain()
{
	return CurrentVillain.Menu;
}

TSubclassOf<AMenuCharacter> FCharacterSelection::NextHero()
{
	if (++CurrentHeroIndex >= IndexedHeroes.Num())
	{
		CurrentHeroIndex = 0;
	}
	CurrentHero = IndexedHeroes[CurrentHeroIndex];
	return CurrentHero.Menu;
}

TSubclassOf<AMenuCharacter> FCharacterSelection::PreviousHero()
{
	if (--CurrentHeroIndex < 0)
	{
		CurrentHeroIndex = IndexedHeroes.Num() - 1;
	}
	CurrentHero = IndexedHeroes[CurrentHeroIndex];
	return CurrentHero.Menu;
}

TSubclassOf<AMenuCharacter> FCharacterSelection::NextVillain()
{
	if (++CurrentVillainIndex >= IndexedVillains.Num())
	{
		CurrentVillainIndex = 0;
	}
	CurrentVillain = IndexedVillains[CurrentVillainIndex];
	return CurrentVillain.Menu;
}

TSubclassOf<AMenuCharacter> FCharacterSelection::PreviousVillain()
{
	if (--CurrentVillainIndex < 0)
	{
		CurrentVillainIndex = IndexedVillains.Num() - 1;
	}
	CurrentVillain = IndexedVillains[CurrentVillainIndex];
	return CurrentVillain.Menu;
}

TSubclassOf<ACharacter> FCharacterSelection::Hero()
{
	return CurrentHero.Game;
}

TSubclassOf<ACharacter> FCharacterSelection::Villain() const
{
	return CurrentVillain.Game;
}
