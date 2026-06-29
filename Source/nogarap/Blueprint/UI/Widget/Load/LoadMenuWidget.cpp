#pragma once

#include "LoadMenuWidget.h"

#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"
#include "nogarap/Blueprint/GameInstance/NogarapGameInstance.h"
#include "nogarap/Blueprint/GameMode/Menu/MainMenuGameMode.h"
#include "nogarap/Data/Save/SaveMeta.h"

void ULoadMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));;
	GameInstance = Cast<UNogarapGameInstance>(GetGameInstance());
	const USaveMeta* SaveMeta = GameInstance->GetSaveMeta();

	ShowLoads(
		FText::FromString(SaveMeta->SlotOne.ToString()),
		FText::FromString(SaveMeta->SlotTwo.ToString()),
		FText::FromString(SaveMeta->SlotThree.ToString()),
		GameInstance->GetCurrentSaveSlot()
	);
}

void ULoadMenuWidget::Load(const int32 GameToLoad) const
{
	GameInstance->SetCurrentSaveSlot(GameToLoad);
	GameMode->SetHeroStats(GameInstance->GetStatsForCurrentCharacter());
}

void ULoadMenuWidget::Back() const
{
	GameMode->HideLoadWidget();
	GameMode->ShowMainWidget();
}
