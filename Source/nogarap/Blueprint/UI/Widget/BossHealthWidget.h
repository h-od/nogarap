#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHealthWidget.generated.h"

UCLASS()
class NOGARAP_API UBossHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealth(float NewValue);
};
