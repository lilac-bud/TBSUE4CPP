// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/TileInfoType.h"
#include "TileInfo.generated.h"


USTRUCT()
struct FTileInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<float> Info;

	FTileInfo() 
	{
		Info.Init(0.0f, ETileInfoType::ChannelsNumber);
	};
};
