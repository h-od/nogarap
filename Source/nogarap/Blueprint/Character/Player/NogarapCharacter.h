#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "nogarap/Data/Character/Gameplay/CharacterGameplay.h"
#include "NogarapCharacter.generated.h"

class UGameOverWidget;
enum class ECharacters : uint8;
class ANogarapGameMode;
class UGameplayComponent;
class ANogarapController;
class UInputAction;

UCLASS()
class NOGARAP_API ANogarapCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY()
	ANogarapGameMode* NogarapGameMode;
	UPROPERTY()
	ANogarapController* NogarapController;

	UPROPERTY(EditDefaultsOnly)
	FCharacterGameplay CharacterGameplay;
	UPROPERTY(EditDefaultsOnly)
	UGameplayComponent* GameplayComponent;

	UPROPERTY()
	float SprintSpeed = 850.0f;
	UPROPERTY()
	float RunSpeed = 600.0f;

	UPROPERTY()
	bool bIsDead = false;
	UPROPERTY()
	bool bIsCounterAttacking = false;

	UPROPERTY()
	float AimStickSens = 50;
	UPROPERTY()
	float AimMouseSens = 50;
	UPROPERTY()
	float StickSens = 50;
	UPROPERTY()
	float MouseSens = 50;

	UPROPERTY()
	bool bSaveAttack;
	UPROPERTY()
	bool bIsAttacking;
	UPROPERTY()
	int32 AttackCount;

	UPROPERTY()
	int32 Combo = 0;
	
	UPROPERTY()
	FTimerHandle WeaponTraceTimerHandle;
	UPROPERTY()
	float WeaponTraceDuration;
	UPROPERTY()
	float WeaponTraceInterval;
	UPROPERTY()
	float WeaponTraceCurrent;
	UPROPERTY()
	TArray<FHitResult> AttackHits;
	UPROPERTY()
	TArray<AActor*> HitActor;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	ECharacters Character;
	
	// InputActions
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Look_MouseAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Look_GamepadAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* AttackAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* BlockAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* HealAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* OffensiveAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* DefensiveAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ToggleViewAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* PauseAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ZoomInAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ZoomOutAction;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Zoom_MouseAction;
	// InputActions - End

	UPROPERTY()
	UAnimMontage* CurrentAnim;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	TArray<UAnimMontage*> AttackAnims;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* BlockAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* OffensiveAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* DefensiveAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* HealAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* HitAnim;
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimationAsset* DeathAnim;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	bool bIsAkimbo;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponBase;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponTip;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponBaseOther;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName WeaponTipOther;

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameOverWidget> GameOverWidgetClass;
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsStunned;

	ANogarapCharacter();

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void SetStats(const FCharacterGameplay& NewCharacterGameplay);
	void SetHealth(float NewValue) const;
	void SetRed(float NewValue) const;
	void SetBlue(float NewValue) const;
	UFUNCTION()
	void Die();
	void SetStamina(float NewValue) const;
	void StaminaDepleted();
	void StopBlocking();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();
	UFUNCTION(BlueprintCallable)
	void SaveAttack();
	UFUNCTION()
	int32 GetScore() const;
	UFUNCTION()
	int32 GetTotalScore() const;

protected:
	UFUNCTION(BlueprintCallable)
	void Hit(AActor* DamagedActor, FVector const& HitFromDirection, FHitResult const& HitInfo, const TSubclassOf<UDamageType> DamageTypeClass) const;

private:
	void MoveTriggered(const FInputActionValue& InputActionValue);
	void MovingStarted();
	void MovingStopped();
	void Look_MouseTriggered(const FInputActionValue& InputActionValue);
	void Look_GamepadTriggered(const FInputActionValue& InputActionValue);
	void SprintStarted();
	void SprintStopped();
	void JumpStarted();
	void JumpStopped();
	void AttackStarted();
	void BlockStarted();
	void BlockStopped();
	void DoAttack();
	void StartWeaponTrace(float Duration);
	void DoWeaponTrace();
	void AttackFinished();
	void HealStarted();
	void OffensiveStarted();
	void DefensiveStarted();
	TArray<AActor*> SphereTraceForEnemy(float Radius);

	ANogarapGameMode* GetGameMode();
};
