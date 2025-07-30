// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/TileInfo.h"
#include "State.generated.h"

/**
 * 
 */
USTRUCT()
struct FState
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FTileInfo> TilesInfo;

	FState() {};
};
