#include "MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "nogarap/Blueprint/Character/Menu/MenuCharacter.h"
#include "nogarap/Blueprint/GameInstance/NogarapGameInstance.h"
#include "nogarap/Blueprint/UI/Widget/Load/LoadMenuWidget.h"
#include "nogarap/Blueprint/UI/Widget/Main/MainMenuWidget.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UNogarapGameInstance>(GetGameInstance());
	MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(GetWorld(), MainMenuWidgetClass));
	MainMenuWidget->AddToViewport();
	APlayerController* Controller = GetGameInstance()->GetLocalPlayers()[0]->GetPlayerController(GetWorld());
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());
	Selection.SetHero(GameInstance->Hero);
	SpawnHero(Selection.MenuHero());
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
	SpawnHero(Selection.NextHero());
}

void AMainMenuGameMode::PreviousHero()
{
	SpawnHero(Selection.PreviousHero());
}

void AMainMenuGameMode::HeroSelected()
{
	GameInstance->HeroToSpawn = Selection.Hero();
	GameInstance->Hero = CurrentHero->Character;

	MainMenuWidget->SetVillainSelect();
}

void AMainMenuGameMode::ShowVillain()
{
	const FTransform HeroTransform(FRotator(0.0f, 270.0f, 0.0f), FVector(-260.0f, 150.0f, 145.0f), FVector(1.0f, 1.0f, 1.0f));

	CurrentHero->SetActorTransform(HeroTransform);
	//CurrentHero->MoveTo(FVector(-260.0f, 150.0f, 145.0f), FRotator(0.0f, 270.0f, 0.0f));

	//TODO eventually have hero walk 
	//TODO also Back from villain to hero & back from confirm to villain

	SpawnVillain(Selection.MenuVillain());
}

void AMainMenuGameMode::NextVillain()
{
	SpawnVillain(Selection.NextVillain());
}

void AMainMenuGameMode::PreviousVillain()
{
	SpawnVillain(Selection.PreviousVillain());
}

void AMainMenuGameMode::VillainSelected() const
{
	MainMenuWidget->VillainSelected();

	GameInstance->VillainToSpawn = Selection.Villain();
}

void AMainMenuGameMode::MoveVillain() const
{
	const FTransform VillainTransform(FRotator(0.0f, 90.0f, 0.0f), FVector(-260.0f, -200.0f, 145.0f), FVector(1.0f, 1.0f, 1.0f));
	CurrentVillain->SetActorTransform(VillainTransform);
	//CurrentVillain->MoveTo(FVector(-260.0f, -200.0f, 145.0f), FRotator(0.0f, 90.0f, 0.0f));
}

void AMainMenuGameMode::Start(const EDifficulty Difficulty) const
{
	GameInstance->SetCurrentCharacterAndDifficulty(CurrentHero->Character, Difficulty);
	APlayerController* Controller = GetGameInstance()->GetLocalPlayers()[0]->GetPlayerController(GetWorld());
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameLevel, false, "");
}

void AMainMenuGameMode::SetHeroStats(const FCharacterStats& Info) const
{
	MainMenuWidget->SetHeroStats(Info);
}

void AMainMenuGameMode::SpawnHero(const TSubclassOf<AMenuCharacter> HeroClass)
{
	if (CurrentHero)
	{
		CurrentHero->Destroy();
	}
	CurrentHero = GetWorld()->SpawnActor<AMenuCharacter>(HeroClass, FTransform());
	MainMenuWidget->SetHeroStats(GameInstance->GetStatsForCharacter(CurrentHero->Character));
}

void AMainMenuGameMode::SpawnVillain(const TSubclassOf<AMenuCharacter> VillainClass)
{
	if (CurrentVillain)
	{
		CurrentVillain->Destroy();
	}
	CurrentVillain = GetWorld()->SpawnActor<AMenuCharacter>(VillainClass, FTransform());
	// GameInstance->GetStatsForCharacter(CurrentVillain->Character);//TODO?
}
