// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Game/GameParameters.h"
#include "TBSGameMode.generated.h"

class IIsPlayer;
class UUserMessageBoard;
class AGrid;
class ATile;

UCLASS()
class TBSUE4CPP_API ATBSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FGameParameters GameParameters;
	UPROPERTY()
	TArray< TScriptInterface<IIsPlayer>> AllPlayers;
	UPROPERTY()
	TArray< TScriptInterface<IIsPlayer>> PlayersInGame;
	int32 CurrentPlayerTurn = 0;
	int32 CurrentIteration = 1;
	UPROPERTY()
	UUserMessageBoard* MessageWidget = nullptr;
	UPROPERTY()
	TArray<int32> PlayersWins;

	void CreatePlayers(AGrid* Grid);
	TArray<ATile*> GetStartingTiles(AGrid* Grid, int32 PlayerIndex) const;
	void ResetGame();

public:
	ATBSGameMode();
	UFUNCTION(BlueprintCallable)
	void Initialize(AGrid* Grid, UUserMessageBoard* MessageWidgetToSet);
	void CycleTurns();
	void PlayerLost(class ATBSPawn* Player);
	void EndGame();
	void StartGame();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
