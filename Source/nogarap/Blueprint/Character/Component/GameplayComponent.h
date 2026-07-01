#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "nogarap/Data/Character/Gameplay/CharacterGameplay.h"
#include "GameplayComponent.generated.h"

struct FCharacterGameplay;
class ANogarapCharacter;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDepletedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopBlockingDelegate);

UCLASS()
class NOGARAP_API UGameplayComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY()
	FCharacterGameplay CharacterStats;
	
	UPROPERTY()
	ANogarapCharacter* Character;
	
	UPROPERTY()
	bool bIsSprinting = false;
	UPROPERTY()
	bool bIsMoving = false;
	UPROPERTY()
	bool bIsBlocking = false;
	UPROPERTY()
	bool bPerfectBlockWindowOpen = false;
	UPROPERTY()
	bool  bGreenActive = false;
	UPROPERTY()
	bool  bRedActive = false;
	UPROPERTY()
	bool  bBlueActive = false;
	UPROPERTY()
	FTimerHandle AbilityTimerHandle;
	
	FStaminaDepletedDelegate StaminaDepletedDelegate;
	FStopBlockingDelegate StopBlockingDelegate;
	
	UPROPERTY()
	FTimerHandle PerfectBlockTimerHandle;
	UPROPERTY()
	FTimerHandle StaminaRegenTimerHandle; //Todo just one stamina timer handle?
	UPROPERTY()
	FTimerHandle StaminaDegenTimerHandle;
	
public:
	UGameplayComponent();

	virtual void BeginPlay() override;

	void SetPlayerManager(const FCharacterGameplay& NewValue);
	
	void TakeDamage(const float DamageToTake, const FVector& DamageDirection);
	void SetIsMoving(bool bMoving);
	void StartSprinting();
	UFUNCTION()
	void StopSprinting();
	void StartStaminaDegen();
	void StartStaminaRegen();

	bool CanAttack() const;
	void Attack();
	void SetBlocking(bool bBlocking);
	bool CanPerfectBlock() const;
	void PerfectBlockWindowClosed();
	UFUNCTION()
	void StopBlocking();

	void UpdateStamina(float Delta);
	
	//heal
	bool CanHeal() const; //todo returns cooldown 
	void HealStart();
	void HealStop();
	//cast
	bool CanOffensive() const; //todo returns cooldown 
	void OffensiveStart();
	void OffensiveStop();
	bool CanDefensive() const; //todo returns cooldown 
	void DefensiveStart();
	void DefensiveStop();
	void Hit(AActor* DamagedActor, const FVector& HitFromDirection, const FHitResult& HitInfo, TSubclassOf<UDamageType> DamageTypeClass);
	int32 GetScore() const;
	int32 UpdateScore(int32 Score);
	int32 GetTotalScore();
	void PerfectBlock();
	float GetDamage() const;

private:
	ANogarapCharacter* GetCharacter();
	
	//Stamina
	void DecrementStamina();
	void IncrementStamina();
};
