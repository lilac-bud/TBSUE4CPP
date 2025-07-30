// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Game/TBSGameInstance.h"
#include "Game/GameParameters.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	PlayersSetting->AddOption(TEXT("Two Human Players"));
	PlayersSetting->AddOption(TEXT("One Human and One Computer Player"));
	PlayersSetting->AddOption(TEXT("Two Computer Players"));
	PlayersSetting->SetSelectedIndex(0);
}

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::SettingsButtonClicked);
	SettingsBackToMenuButton->OnClicked.AddDynamic(this, &UMainMenu::SettingsBackToMenuButtonClicked);
	SaveSettingsButton->OnClicked.AddDynamic(this, &UMainMenu::SaveSettingsButtonClicked);
}

void UMainMenu::SettingsButtonClicked()
{
	check(MenuSwitcher->HasChild(Settings));
	MenuSwitcher->SetActiveWidget(Settings);
}

void UMainMenu::SettingsBackToMenuButtonClicked()
{
	check(MenuSwitcher->HasChild(MainMenu));
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::SaveSettingsButtonClicked()
{
	FGameParameters& GameParameters = GetWorld()->GetGameInstance<UTBSGameInstance>()->GameParameters;
	int32 SelectedPlayerSetting = PlayersSetting->GetSelectedIndex();
	GameParameters.HumanPlayersAmount = 2 - SelectedPlayerSetting;
	GameParameters.ComputerPlayersAmount = SelectedPlayerSetting;
	GameParameters.IterationsAmount = (int32)IterationsAmount->GetValue();
	check(MenuSwitcher->HasChild(MainMenu));
	MenuSwitcher->SetActiveWidget(MainMenu);
}