#pragma once

#include "CoreMinimal.h"
#include "nogarap/Blueprint/Character/Enemy/EnemyCharacter.h"
#include "VillainCharacter.generated.h"

class UBossHealthWidget;

UCLASS()
class NOGARAP_API AVillainCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
	UPROPERTY()
	UBossHealthWidget* HealthWidget;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBossHealthWidget> HealthWidgetClass;
	
public:
	AVillainCharacter();

	virtual void BeginPlay() override;

protected:
	virtual void SetHealth(float NewValue) override;
	UBossHealthWidget* GetBossWidget();
};
