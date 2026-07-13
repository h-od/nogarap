#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class ANogarapGameMode;
class UEnemyHealthWidget;

UCLASS()
class NOGARAP_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
	bool bShouldAttackRight = false;
	UPROPERTY()
	FTimerHandle WeaponTraceTimerHandle;
	UPROPERTY()
	float WeaponTraceDuration;
	UPROPERTY()
	float WeaponTraceInterval;
	UPROPERTY()
	float WeaponTraceCurrent;

	UPROPERTY()
	FHitResult AttackHitRight;
	UPROPERTY()
	FHitResult AttackHitLeft;
	
	UPROPERTY()
	FTimerHandle RagdollTimerHandle;
	UPROPERTY()
	FTimerHandle DestroyTimerHandle;
	
	UPROPERTY() 
	float Health = 1.0f;
	UPROPERTY()
	float MaxHealth = 1.0f;
	UPROPERTY()
	float DealDamage = 1.0f;

protected:
	UPROPERTY()
	ANogarapGameMode* GameMode;

	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* AttackRightAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* AttackLeftAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* HitAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* StunnedAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimationAsset* DeadAnim;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponBaseRight;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponTipRight;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponBaseLeft;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponTipLeft;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Output") //STT
	bool bIsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Output") //STT
	bool bIsStunned = false;
	
	AEnemyCharacter();

	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void SetStats(float NewDamage, float NewHealth);

	UFUNCTION(BlueprintCallable)
	float Attack();
	UFUNCTION(BlueprintCallable)
	void StartStun();
	UFUNCTION(BlueprintCallable)
	void StopStun();
	
	void HitGreen(const FVector& Vector);
	void HitBlue(const FVector& Vector);
	void HitRed(const FVector& Vector);

	
protected:
	virtual void SetHealth(float NewValue) PURE_VIRTUAL(AEnemyCharacter::SetHealth);

	UFUNCTION()
	void UpdateHealth(float Delta);
	void RagDoll() const;
	void DelayedDestroy(float Duration);
	UFUNCTION()
	void DoDestroy();

private:
	void StartWeaponTrace(float Duration);
	void DoWeaponTrace();
	void AttackFinished();
};
