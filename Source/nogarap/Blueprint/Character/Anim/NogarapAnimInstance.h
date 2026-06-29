#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NogarapAnimInstance.generated.h"

class ANogarapCharacter;

UCLASS()
class NOGARAP_API UNogarapAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	UPROPERTY()
	FRotator RotationLastTick;

protected:
	UPROPERTY(BlueprintReadOnly)
	ANogarapCharacter* Character;
	
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	bool IsInAir;
	UPROPERTY(BlueprintReadOnly)
	float Pitch;
	UPROPERTY(BlueprintReadOnly)
	float Yaw;
	UPROPERTY(BlueprintReadOnly)
	float YawDelta;
	UPROPERTY(BlueprintReadOnly)
	bool IsAccelerating;
	UPROPERTY(BlueprintReadOnly)
	bool FullBody;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
};
