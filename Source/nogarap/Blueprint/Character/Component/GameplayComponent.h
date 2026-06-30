#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "nogarap/Data/Character/CharacterGameplay.h"
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

	FCharacterGameplay GetPlayerManager() const;
	void SetPlayerManager(const FCharacterGameplay& NewValue);
	
	void TakeDamage(const float DamageToTake, const FVector& DamageDirection);
	void SetIsMoving(bool bMoving);
	void StartSprinting();
	UFUNCTION()
	void StopSprinting();
	void StartStaminaDegen();
	void StartStaminaRegen();

	bool CanAttack();
	void Attack();
	void SetBlocking(bool bBlocking);
	bool CanPerfectBlock() const;
	void PerfectBlockWindowClosed();
	UFUNCTION()
	void StopBlocking();

	void UpdateStamina(float Delta);
	
	//heal
	bool CanHeal() const; //todo returns cooldown 
	void Heal();
	//cast
	bool CanOffensive() const; //todo returns cooldown 
	void Offensive();
	bool CanDefensive() const; //todo returns cooldown 
	void Defensive();
	void Hit(AActor* DamagedActor, const FVector& HitFromDirection, const FHitResult& HitInfo, TSubclassOf<UDamageType> DamageTypeClass);
	int32 GetScore() const;
	int32 UpdateScore(int32 Score);
	int32 GetTotalScore();

private:
	ANogarapCharacter* GetCharacter();
	
	//Stamina
	void DecrementStamina();
	void IncrementStamina();
};
