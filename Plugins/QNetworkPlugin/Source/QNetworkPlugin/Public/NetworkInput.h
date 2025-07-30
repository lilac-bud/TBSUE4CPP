// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkInput.generated.h"

USTRUCT()
struct FNetworkInput
{
	GENERATED_BODY()

	float Reward;
	UPROPERTY()
	TArray<float> State;
	UPROPERTY()
	TArray<float> Actions;
	int32 ActionsNumber;

	FNetworkInput() : Reward(0), ActionsNumber(0) {};
};
