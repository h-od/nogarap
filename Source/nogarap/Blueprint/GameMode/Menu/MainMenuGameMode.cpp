#include "MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "nogarap/Blueprint/Character/Menu/MenuCharacter.h"
#include "nogarap/Blueprint/GameInstance/NogarapGameInstance.h"
#include "nogarap/Blueprint/UI/Widget/MainMenuWidget.h"
#include "nogarap/Blueprint/UI/Widget/Load/LoadMenuWidget.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UNogarapGameInstance>(GetGameInstance());
	MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(GetWorld(), MainMenuWidgetClass));
	MainMenuWidget->AddToViewport();
	APlayerController* Controller = GetGameInstance()->GetLocalPlayers()[0]->GetPlayerController(GetWorld());
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());
	Heroes.GetKeys(HeroKeys);
	Villains.GetKeys(VillainKeys);
	SpawnHero();
}

void AMainMenuGameMode::ShowMainWidget()
{
	if (!MainMenuWidget)
	{
		MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(GetWorld(), MainMenuWidgetClass));
		MainMenuWidget->AddToViewport();
	}
	else
	{
		MainMenuWidget->AddToViewport();
		MainMenuWidget->Unfade();
	}
}

void AMainMenuGameMode::ShowLoadWidget()
{
	if (!LoadMenuWidget)
	{
		LoadMenuWidget = Cast<ULoadMenuWidget>(CreateWidget(GetWorld(), LoadWidgetClass));
	}
	LoadMenuWidget->AddToViewport();
}

void AMainMenuGameMode::ShowSkillsWidget()
{
	if (!SkillsMenuWidget)
	{
		SkillsMenuWidget = Cast<USkillsMenuWidget>(CreateWidget(GetWorld(), SkillsWidgetClass));
	}
	SkillsMenuWidget->AddToViewport();
}

void AMainMenuGameMode::HideMainWidget() const
{
	MainMenuWidget->RemoveFromParent();
}

void AMainMenuGameMode::HideLoadWidget() const
{
	LoadMenuWidget->RemoveFromParent();
}

void AMainMenuGameMode::HideSkillsWidget() const
{
	SkillsMenuWidget->RemoveFromParent();
}

FCharacterStats AMainMenuGameMode::CurrentSavedHeroStats() const
{
	return GameInstance->GetStatsForCharacter(CurrentHero->Character);
}

void AMainMenuGameMode::NextHero()
{
	if (++HeroIndex >= Heroes.Num())
	{
		HeroIndex = 0;
	}
	SpawnHero();
}

void AMainMenuGameMode::PreviousHero()
{
	if (--HeroIndex < 0)
	{
		HeroIndex = Heroes.Num() - 1;
	}
	SpawnHero();
}

void AMainMenuGameMode::HeroSelected()
{
	GameInstance->HeroToSpawn = Heroes[HeroKeys[HeroIndex]];
	GameInstance->Hero = CurrentHero->Character;

	MainMenuWidget->SetVillainSelect();
}

void AMainMenuGameMode::ShowVillain()
{
	const FTransform HeroTransform(FRotator(0.0f, 270.0f, 0.0f), FVector(-260.0f, 150.0f, 145.0f), FVector(1.0f, 1.0f, 1.0f));

	CurrentHero->SetActorTransform(HeroTransform);

	//TODO eventually have hero walk 
	//TODO also Back from villain to hero & back from confirm to villain

	SpawnVillain();
}

void AMainMenuGameMode::NextVillain()
{
	if (++VillainIndex >= Villains.Num())
	{
		VillainIndex = 0;
	}
	SpawnVillain();
}

void AMainMenuGameMode::PreviousVillain()
{
	if (--VillainIndex < 0)
	{
		VillainIndex = Villains.Num() - 1;
	}
	SpawnVillain();
}

void AMainMenuGameMode::VillainSelected() const
{
	MainMenuWidget->VillainSelected();

	GameInstance->VillainToSpawn = Villains[VillainKeys[VillainIndex]];
}

void AMainMenuGameMode::MoveVillain() const
{
	const FTransform VillainTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(-260.0f, -200.0f, 145.0f), FVector(1.0f, 1.0f, 1.0f));
	CurrentVillain->SetActorTransform(VillainTransform);
}

void AMainMenuGameMode::Start() const
{
	APlayerController* Controller = GetGameInstance()->GetLocalPlayers()[0]->GetPlayerController(GetWorld());
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameLevel, false, "");
}

void AMainMenuGameMode::SetHeroStats(const FCharacterStats& Info) const
{
	MainMenuWidget->SetHeroStats(Info);
}

void AMainMenuGameMode::SpawnHero()
{
	if (CurrentHero)
	{
		CurrentHero->Destroy();
	}
	CurrentHero = GetWorld()->SpawnActor<AMenuCharacter>(HeroKeys[HeroIndex], FTransform());
	MainMenuWidget->SetHeroStats(GameInstance->GetStatsForCharacter(CurrentHero->Character));
}

void AMainMenuGameMode::SpawnVillain()
{
	if (CurrentVillain)
	{
		CurrentVillain->Destroy();
	}
	CurrentVillain = GetWorld()->SpawnActor<AMenuCharacter>(VillainKeys[VillainIndex], FTransform());
	GameInstance->GetStatsForCharacter(CurrentVillain->Character);
}
