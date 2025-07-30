// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TBSGameInstance.h"

UTBSGameInstance::UTBSGameInstance()
{
	GameParameters.HumanPlayersAmount = 2;
	GameParameters.ComputerPlayersAmount = 0;
	GameParameters.IterationsAmount = 1;
	GameParameters.StartingUnitsAmount = 3;
}