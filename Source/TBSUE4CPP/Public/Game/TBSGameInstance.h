// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Game/GameParameters.h"
#include "TBSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TBSUE4CPP_API UTBSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	FGameParameters GameParameters;

	UTBSGameInstance();
};
