#pragma once

#include "CoreMinimal.h"
#include "nogarap/Blueprint/Character/Enemy/EnemyCharacter.h"
#include "MinionCharacter.generated.h"

class ANogarapGameMode;

UCLASS()
class NOGARAP_API AMinionCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite)
	UEnemyHealthWidget* HealthWidget;
	
public:
	UPROPERTY()
	int32 SpawnPoint;
	
	virtual void BeginPlay() override;
	void SetStats(int32 NewHealth, int32 NewDamage);

protected:
	virtual void SetHealth(float NewValue) override;
};
