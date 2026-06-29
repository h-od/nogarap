#include "NogarapAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "nogarap/Blueprint/Character/Player/NogarapCharacter.h"

void UNogarapAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Character = Cast<ANogarapCharacter>(TryGetPawnOwner());
}

void UNogarapAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (const APawn* Owner = TryGetPawnOwner())
	{
		const UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(Owner->GetMovementComponent());
		IsInAir = MovementComponent->IsFalling();
		Speed = Owner->GetVelocity().Size();
		const FRotator ActorRotation = Owner->GetActorRotation();
		FRotator Delta = Owner->GetBaseAimRotation() - ActorRotation;
		Delta.Normalize();
		Pitch = Delta.Pitch;
		Yaw = Delta.Yaw;

		FRotator DeltaSinceLastTick = RotationLastTick - ActorRotation;
		DeltaSinceLastTick.Normalize();
		YawDelta = UKismetMathLibrary::FInterpTo(YawDelta, (DeltaSinceLastTick.Yaw / DeltaSeconds) / 7.0f, DeltaSeconds, 6.0f);
		RotationLastTick = ActorRotation;

		if (MovementComponent)
		{
			IsAccelerating = MovementComponent->GetCurrentAcceleration().Size() > 0.0f;
		}
	}

	FullBody = GetCurveValue(FName("FullBody")) > 0.0f;
}
