#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadMenuWidget.generated.h"

class AMainMenuGameMode;
class UNogarapGameInstance;

UCLASS()
class NOGARAP_API ULoadMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UNogarapGameInstance* GameInstance;
	UPROPERTY()
	AMainMenuGameMode* GameMode;
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowLoads(const FText& LoadOne,const FText& LoadTwo,const FText& LoadThree, const int32 CurrentSlot);
	
	UFUNCTION(BlueprintCallable)
	void Load(int32 GameToLoad) const;
	
protected:
	UFUNCTION(BlueprintCallable)
	void Back() const;
};

