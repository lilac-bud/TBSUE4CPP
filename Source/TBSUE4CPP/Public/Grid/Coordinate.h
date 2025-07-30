// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Coordinate.generated.h"

USTRUCT(BlueprintType)
struct FCoordinate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Col;

	FCoordinate() :Row(0), Col(0) {}
	FCoordinate(int32 RowToSet, int32 ColToSet) :Row(RowToSet), Col(ColToSet) {};
};