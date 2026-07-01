#include "NogarapHUD.h"

#include "Blueprint/UserWidget.h"
#include "nogarap/Blueprint/UI/Widget/GameOver/GameOverWidget.h"
#include "nogarap/Blueprint/UI/Widget/PlayerStatsWidget.h"

void ANogarapHUD::BeginPlay()
{
	Super::BeginPlay();
	GetPlayerWidget()->AddToViewport();
}

void ANogarapHUD::SetScore(const int32 NewValue)
{
	GetPlayerWidget()->SetScore(FText::FromString(FString::FromInt(NewValue)));
}

void ANogarapHUD::SetCombo(const int32 NewValue)
{
	GetPlayerWidget()->SetCombo(FText::FromString(FString::FromInt(NewValue)));
}

void ANogarapHUD::SetPlayerHealth(const float NewValue)
{
	GetPlayerWidget()->SetHealth(NewValue);
}

void ANogarapHUD::SetPlayerStamina(const float NewValue)
{
	GetPlayerWidget()->SetStamina(NewValue);
}

void ANogarapHUD::SetRed(const float NewValue)
{
	GetPlayerWidget()->SetChargeRed(NewValue);
}

void ANogarapHUD::SetBlue(const float NewValue)
{
	GetPlayerWidget()->SetChargeBlue(NewValue);
}

void ANogarapHUD::SetWave(const int32 NewValue)
{
	GetPlayerWidget()->SetWave(FText::FromString(FString::FromInt(NewValue + 1)));
}

void ANogarapHUD::ShowGameOver() const
{
	CreateWidget(GetWorld(), GameOverWidgetClass)->AddToViewport();
}

UPlayerStatsWidget* ANogarapHUD::GetPlayerWidget()
{
	if (!PlayerStatsWidget)
	{
		PlayerStatsWidget = Cast<UPlayerStatsWidget>(CreateWidget(GetWorld(), PlayerStatsWidgetClass));
	}

	return PlayerStatsWidget;
}
