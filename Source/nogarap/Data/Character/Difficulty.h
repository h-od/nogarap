#pragma once

#include "Difficulty.generated.h"

UENUM()
enum class EDifficulty : uint8
{
	Easy UMETA(DisplayName = "Easy"),
	Medium UMETA(DisplayName = "Medium"),
	Hard UMETA(DisplayName = "Hard"),
	Expert UMETA(DisplayName = "Expert")
};
