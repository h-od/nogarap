#include "NogarapController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "nogarap/Blueprint/UI/HUD/NogarapHUD.h"

void ANogarapController::BeginPlay()
{
	Super::BeginPlay();
	HUD = Cast<ANogarapHUD>(GetHUD());
	
	HUD->SetPlayerHealth(1.0f);
	HUD->SetPlayerStamina(1.0f);
	HUD->SetRed(0.0f);
	HUD->SetBlue(0.0f);
}

void ANogarapController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (const UInputMappingContext* Context : MappingContexts)
			{
				Subsystem->AddMappingContext(Context, 0);
			}
		}
	}
}

void ANogarapController::SetHealth(const float NewValue) const
{
	HUD->SetPlayerHealth(NewValue);
}

void ANogarapController::SetStamina(const float NewValue) const
{
	HUD->SetPlayerStamina(NewValue);
}

void ANogarapController::SetBlue(const float NewValue) const
{
	HUD->SetBlue(NewValue);
}

void ANogarapController::SetRed(const float NewValue) const
{
	HUD->SetRed(NewValue);
}

void ANogarapController::SetScore(const int32 NewValue) const
{
	HUD->SetScore(NewValue);
}

void ANogarapController::SetCombo(const int32 NewValue) const
{
	HUD->SetCombo(NewValue);
}

void ANogarapController::SetWave(const int32 NewValue) const
{
	HUD->SetWave(NewValue);
}

void ANogarapController::GameOver() const
{
	HUD->ShowGameOver();
}
