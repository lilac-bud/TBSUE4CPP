// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerPawn.h"
#include "Game/TBSGameMode.h"

// Sets default values
APlayerPawn::APlayerPawn() : ATBSPawn() {}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerPawn::Initialize(int32 PlayerIDToSet)
{
	Super::Initialize(PlayerIDToSet);
	const UWorld* World = GetWorld();
	check(IsValid(World));
	PlayerController = World->GetFirstPlayerController();
}

void APlayerPawn::Turn()
{
	Super::Turn();
	check(IsValid(PlayerController));
	PlayerController->Possess(this);
}

void APlayerPawn::LoseCondition()
{
	check(IsValid(GameMode));
	GameMode->PlayerLost(this);
}

void APlayerPawn::Win()
{
	GetController()->UnPossess();
	check(IsValid(GameMode));
	GameMode->EndGame();
}