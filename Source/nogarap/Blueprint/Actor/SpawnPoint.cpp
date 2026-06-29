#include "SpawnPoint.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

FTransform ASpawnPoint::GetSpawnPoint_Implementation()
{
	return FTransform();
}
