// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPawn.h"
#include "Units/UnitsComponent.h"
#include "Units/Unit.h"
#include "QNetworkComponent.h"
#include "Game/TBSGameMode.h"
#include "Grid/Grid.h"
#include "Grid/Tile.h"
#include "Units/TileUnitInfo.h"
#include "AI/TileInfo.h"
#include "AI/TileInfoType.h"
#include "NetworkInput.h"

using Reward = float;

AAIPawn::AAIPawn() : ATBSPawn()
{
	QNetworkComponent = CreateDefaultSubobject<UQNetworkComponent>(TEXT("QNetworkComponent"));
}

void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AAIPawn::Initialize(int32 PlayerIDToSet, AGrid* GridFromLevel)
{
	Super::Initialize(PlayerIDToSet);
	Grid = GridFromLevel;
	CurrentState.TilesInfo.Init(FTileInfo(), Grid->GridSize.Row * Grid->GridSize.Col);
	QNetworkComponent->Initialize(Grid->GridSize.Row, Grid->GridSize.Col, ETileInfoType::ChannelsNumber, 
		PlayerID, FPaths::ProjectContentDir());
}

void AAIPawn::UpdateCurrentState()
{
	for (ATile* Tile : Grid->Tiles)
	{
		FTileInfo& TileInfo = CurrentState.TilesInfo[Tile->TileID];
		float OwnUnitHealth = 0.0f;
		float EnemyUnitHealth = 0.0f;
		switch (UnitsComponent->CheckForUnits(Tile))
		{
		case ETileUnitInfo::OwnUnit:
			OwnUnitHealth = Tile->Unit->GetHealth();
			break;
		case ETileUnitInfo::EnemyUnit:
			EnemyUnitHealth = Tile->Unit->GetHealth();
			break;
		default:
			break;
		}
		TileInfo.Info[ETileInfoType::OwnUnitHealth] = OwnUnitHealth;
		TileInfo.Info[ETileInfoType::EnemyUnitHealth] = EnemyUnitHealth;
	}
}

void AAIPawn::GenerateActions(const AUnit* SelectedUnit)
{
	PossibleActions.Reserve(SelectedUnit->ReachableTiles.Num());
	for (ATile* TargetTile : SelectedUnit->ReachableTiles)
	{
		FState NewAction = CurrentState;
		if (TargetTile != SelectedUnit->GetTile())
		{
			FTileInfo& TargetTileInfo = NewAction.TilesInfo[TargetTile->TileID];
			FTileInfo* DestinationTileInfo = nullptr;
			float SubstructResult = TargetTileInfo.Info[ETileInfoType::EnemyUnitHealth] - SelectedUnit->AttackDamage;
			if (SubstructResult > 0)
			{
				TargetTileInfo.Info[ETileInfoType::EnemyUnitHealth] = SubstructResult;
				if (!SelectedUnit->GetTile()->Neighbours.Contains(TargetTile))
					DestinationTileInfo = &NewAction.TilesInfo[SelectedUnit->FindCloserTile(TargetTile)->TileID];
			}
			else
			{
				DestinationTileInfo = &TargetTileInfo;
				TargetTileInfo.Info[ETileInfoType::EnemyUnitHealth] = 0;
			}
			if (DestinationTileInfo)
			{
				DestinationTileInfo->Info[ETileInfoType::OwnUnitHealth] = SelectedUnit->GetHealth();
				NewAction.TilesInfo[SelectedUnit->GetTile()->TileID].Info[ETileInfoType::OwnUnitHealth] = 0;
			}
		}
		PossibleActions.Add(NewAction);
	}
}

FNetworkInput AAIPawn::BuildNetworkInput(float Reward) const
{
	FNetworkInput NetworkInput;
	NetworkInput.Reward = Reward;
	NetworkInput.ActionsNumber = PossibleActions.Num();
	NetworkInput.State.Reserve(Grid->GridSize.Row * Grid->GridSize.Col * ETileInfoType::ChannelsNumber);
	for (const FTileInfo& TileInfo : CurrentState.TilesInfo)
		NetworkInput.State.Append(TileInfo.Info);
	NetworkInput.Actions.Reserve(NetworkInput.ActionsNumber * NetworkInput.State.Num());
	for (const FState& PossibleAction : PossibleActions)
		for (const FTileInfo& TileInfo : PossibleAction.TilesInfo)
			NetworkInput.Actions.Append(TileInfo.Info);
	return NetworkInput;
}

void AAIPawn::Turn()
{
	Super::Turn();
	UpdateCurrentState();
	for (AUnit* SelectedUnit : UnitsComponent->Units)
	{
		if (SelectedUnit->ReachableTiles.Num() <= 1) continue;
		GenerateActions(SelectedUnit);
		int32 ActionIndex = QNetworkComponent->CallQNetwork(BuildNetworkInput(Reward{ 0.0f }));
		//int32 ActionIndex = FMath::RandRange(0, PossibleActions.Num() - 1);
		CurrentState = PossibleActions[ActionIndex];
		PossibleActions.Empty();
		ATile* TargetTile = SelectedUnit->ReachableTiles[ActionIndex];
		if (TargetTile != SelectedUnit->GetTile())
			SelectedUnit->Act(TargetTile);
		if (!bCanPlay) return;
	}
	FTimerHandle UnusedHandle;
	check(IsValid(GameMode));
	GetWorldTimerManager().SetTimer(UnusedHandle, GameMode, &ATBSGameMode::CycleTurns, 0.01f, false);
}

void AAIPawn::LoseCondition()
{
	bCanPlay = false;
	UpdateCurrentState();
	QNetworkComponent->CallQNetwork(BuildNetworkInput(Reward{ 0.0f }));
	check(IsValid(GameMode));
	GameMode->PlayerLost(this);
}

void AAIPawn::Win()
{
	bCanPlay = false;
	//UpdateCurrentState();
	QNetworkComponent->CallQNetwork(BuildNetworkInput(Reward{ 1.0f }));
	FTimerHandle UnusedHandle;
	check(IsValid(GameMode));
	GetWorldTimerManager().SetTimer(UnusedHandle, GameMode, &ATBSGameMode::EndGame, 0.5f, false);
}

void AAIPawn::Reset()
{
	Super::Reset();
	QNetworkComponent->SoftReset();
	bCanPlay = true;
}