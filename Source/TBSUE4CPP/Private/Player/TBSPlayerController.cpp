// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TBSPlayerController.h"
#include "Game/TBSGameMode.h"
#include "Grid/Tile.h"
#include "Units/UnitsComponent.h"
#include "Units/TileUnitInfo.h"
#include "Units/Unit.h"
#include "Interfaces/IsPlayer.h"

ATBSPlayerController::ATBSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bAutoManageActiveCameraTarget = false;
}

void ATBSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	check(IsValid(World));
	GameMode = Cast<ATBSGameMode>(World->GetAuthGameMode());
}

void ATBSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("IA_Click"), IE_Pressed, this, &ATBSPlayerController::Click);
	InputComponent->BindAction(TEXT("IA_EndTurn"), IE_Pressed, this, &ATBSPlayerController::EndTurn);
}

void ATBSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	bCanPlay = true;
	UnitsComponent = (UUnitsComponent*)InPawn->GetComponentByClass(UUnitsComponent::StaticClass());
}

void ATBSPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	bCanPlay = false;
	UnitsComponent = nullptr;
}

void ATBSPlayerController::EndTurn()
{
	if (!bCanPlay) return;
	UnPossess();
	check(IsValid(GameMode));
	GameMode->CycleTurns();
}

void ATBSPlayerController::Click()
{
	if (!bCanPlay) return;
	for (ATile* Tile : HighlightedTiles)
		Tile->ChangeTileColor(false);
	HighlightedTiles.Empty();
	ATile* ClickedTile = GetClickedTile();
	if (ClickedTile && ClickedTile != SelectedTile)
	{
		SelectedTile = ClickedTile;
		check(IsValid(UnitsComponent));
		switch (UnitsComponent->CheckForUnits(SelectedTile))
		{
		case ETileUnitInfo::NoUnits:
			//pass to the EnemyUnit case
		case ETileUnitInfo::EnemyUnit:
			if (IsValid(SelectedUnit))
				SelectedUnit->Act(SelectedTile);
			break;
		case ETileUnitInfo::OwnUnit:
			SelectedUnit = SelectedTile->Unit;
			HighlightedTiles = SelectedUnit->ReachableTiles;
			for (ATile* Tile : HighlightedTiles)
				Tile->ChangeTileColor(true);
			return;
		default:
			break;
		}
	}
	SelectedTile = nullptr;
	SelectedUnit = nullptr;
}

ATile* ATBSPlayerController::GetClickedTile() const
{
	FHitResult HitResult;
	if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, HitResult))
		return Cast<ATile>(HitResult.Actor);
	else return nullptr;
}