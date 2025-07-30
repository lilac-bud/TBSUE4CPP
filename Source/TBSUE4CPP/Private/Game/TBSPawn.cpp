// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TBSPawn.h"
#include "Units/UnitsComponent.h"
#include "Game/TBSGameMode.h"

// Sets default values
ATBSPawn::ATBSPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	UnitsComponent = CreateDefaultSubobject<UUnitsComponent>(TEXT("UnitsComponent"));
}

// Called when the game starts or when spawned
void ATBSPawn::BeginPlay()
{
	Super::BeginPlay();

}

void ATBSPawn::Initialize(int32 PlayerIDToSet)
{
	PlayerID = PlayerIDToSet;
	UWorld* World = GetWorld();
	check(IsValid(World));
	GameMode = Cast<ATBSGameMode>(World->GetAuthGameMode());
}

int32 ATBSPawn::GetPlayerID() const
{
	return PlayerID;
}

void ATBSPawn::Turn()
{
	UnitsComponent->NewTurn();
}

void ATBSPawn::Reset()
{
	UnitsComponent->ClearUnits();
	UnitsComponent->AddStartingUnits();
}