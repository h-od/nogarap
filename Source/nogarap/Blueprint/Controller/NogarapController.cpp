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
	HUD->SetChargeRed(1.0f);
	HUD->SetChargeBlue(1.0f);
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

void ANogarapController::GameOver()
{
	HUD->ShowGameSummary();
}
