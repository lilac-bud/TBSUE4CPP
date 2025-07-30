// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileUnitInfo.generated.h"

/**
 *
 */
UENUM()
enum class ETileUnitInfo : uint8
{
	NoUnits,
	OwnUnit,
	EnemyUnit
};