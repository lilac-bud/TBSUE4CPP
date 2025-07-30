// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TBSGameMode.h"
#include "Game/TBSGameInstance.h"
#include "Units/UnitsComponent.h"
#include "Grid/Grid.h"
#include "Interfaces/IsPlayer.h"
#include "UI/UserMessageBoard.h"
#include "Player/TBSPlayerController.h"
#include "Game/TBSPawn.h"
#include "Player/PlayerPawn.h"
#include "AI/AIPawn.h"

ATBSGameMode::ATBSGameMode()
{
	PlayerControllerClass = ATBSPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;
}

void ATBSGameMode::Initialize(AGrid* Grid, UUserMessageBoard* MessageWidgetToSet)
{
	MessageWidget = MessageWidgetToSet;
	GameParameters = GetWorld()->GetGameInstance<UTBSGameInstance>()->GameParameters;
	CreatePlayers(Grid);
	if (GameParameters.IterationsAmount > 1)
		MessageWidget->SetIterationsMessage(CurrentIteration);
	StartGame();
}

void ATBSGameMode::CreatePlayers(AGrid* Grid)
{
	const int32 PlayersAmount = GameParameters.HumanPlayersAmount + GameParameters.ComputerPlayersAmount;
	AllPlayers.Reserve(PlayersAmount);
	PlayersWins.Init(0, PlayersAmount);
	UWorld* World = GetWorld();
	check(IsValid(World));
	for (int32 PlayerID = 0; PlayerID < PlayersAmount; PlayerID++)
	{
		ATBSPawn* Player;
		if (PlayerID < GameParameters.HumanPlayersAmount)
		{
			APlayerPawn* HumanPlayer = World->SpawnActor<APlayerPawn>();
			HumanPlayer->Initialize(PlayerID);
			Player = HumanPlayer;
		}
		else
		{
			AAIPawn* ComputerPlayer = World->SpawnActor<AAIPawn>();
			ComputerPlayer->Initialize(PlayerID, Grid);
			Player = ComputerPlayer;
		}
		UUnitsComponent* UnitsComponent = (UUnitsComponent*)Player->GetComponentByClass(UUnitsComponent::StaticClass());
		if (UnitsComponent)
		{
			UnitsComponent->StartingTiles = GetStartingTiles(Grid, PlayerID);
			UnitsComponent->AddStartingUnits();
		}
		AllPlayers.Add(Player);
	}
}

TArray<ATile*> ATBSGameMode::GetStartingTiles(AGrid* Grid, int32 PlayerIndex) const
{
	TArray<ATile*> StartingTiles;
	StartingTiles.Reserve(GameParameters.StartingUnitsAmount);
	int32 Begin = 0, End = 0;
	if (PlayerIndex)
	{
		End = Grid->Tiles.Num();
		Begin = End - GameParameters.StartingUnitsAmount;
	}
	else
		End = GameParameters.StartingUnitsAmount;
	for (int32 i = Begin; i < End; i++)
		StartingTiles.Add(Grid->Tiles[i]);
	return StartingTiles;
}

void ATBSGameMode::StartGame()
{
	FTimerHandle UnusedHandle;
	PlayersInGame = AllPlayers;
	CurrentPlayerTurn = FMath::RandRange(0, PlayersInGame.Num() - 1);
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ATBSGameMode::CycleTurns, 1.0f, false);
}

void ATBSGameMode::CycleTurns()
{
	CurrentPlayerTurn++;
	if (CurrentPlayerTurn >= PlayersInGame.Num())
		CurrentPlayerTurn = 0;
	IIsPlayer* CurrentPlayer = (IIsPlayer*)PlayersInGame[CurrentPlayerTurn].GetInterface();
	MessageWidget->SetTurnMessage(CurrentPlayer->GetPlayerID());
	CurrentPlayer->Turn();
}

void ATBSGameMode::PlayerLost(ATBSPawn* Player)
{
	PlayersInGame.Remove(Player);
	CurrentPlayerTurn--;
	if (PlayersInGame.Num() == 1)
	{
		IIsPlayer* WinningPlayer = (IIsPlayer*)PlayersInGame[0].GetInterface();
		const int32 PlayerID = WinningPlayer->GetPlayerID();
		PlayersWins[PlayerID]++;
		MessageWidget->SetWinMessage(PlayerID);
		WinningPlayer->Win();
	}
}

void ATBSGameMode::EndGame()
{
	if (CurrentIteration < GameParameters.IterationsAmount)
	{
		CurrentIteration++;
		MessageWidget->SetIterationsMessage(CurrentIteration);
		ResetGame();
	}
	else
		MessageWidget->EnableExitToMenuButton();
}

void ATBSGameMode::ResetGame()
{
	MessageWidget->RemoveWinMessage();
	for (TScriptInterface<IIsPlayer> Player : AllPlayers)
		((IIsPlayer*)Player.GetInterface())->Reset();
	StartGame();
}

void ATBSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	FString StringToSave = TEXT("PlayersWins: ");
	for (int32 WinCount : PlayersWins)
	{
		StringToSave.AppendInt(WinCount);
		StringToSave += TEXT(" ");
	}
	StringToSave += LINE_TERMINATOR;
	FFileHelper::SaveStringToFile(StringToSave, *(FPaths::ProjectContentDir() + TEXT("Results.txt")),
		FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}