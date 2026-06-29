#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthWidget.generated.h"

UCLASS()
class NOGARAP_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealth(float NewValue);
};
