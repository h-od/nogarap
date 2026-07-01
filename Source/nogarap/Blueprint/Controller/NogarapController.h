#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "nogarap/Blueprint/GameMode/NogarapGameMode.h"
#include "NogarapController.generated.h"

class ANogarapHUD;
class UInputMappingContext;

UCLASS()
class NOGARAP_API ANogarapController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY()
	ANogarapHUD* HUD;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input Mappings")
	TArray<UInputMappingContext*> MappingContexts;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
public:
	void SetHealth(float NewValue) const;
	void SetStamina(float NewValue) const;
	void SetBlue(float NewValue) const;
	void SetRed(float NewValue) const;
	void SetScore(int32 NewValue) const;
	void SetCombo(int32 NewValue) const;
	void SetWave(int32 NewValue) const;
	void GameOver() const;
};
