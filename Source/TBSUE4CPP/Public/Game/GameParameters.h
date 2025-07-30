// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameParameters.generated.h"

USTRUCT()
struct FGameParameters
{
	GENERATED_BODY()

	int32 HumanPlayersAmount;
	int32 ComputerPlayersAmount;
	int32 IterationsAmount;
	int32 StartingUnitsAmount;

	FGameParameters() :HumanPlayersAmount(0), ComputerPlayersAmount(0), IterationsAmount(0), StartingUnitsAmount(0) {};
};