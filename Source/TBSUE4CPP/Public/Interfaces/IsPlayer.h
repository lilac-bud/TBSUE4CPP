// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsPlayer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TBSUE4CPP_API IIsPlayer
{

	GENERATED_BODY()

public:
	virtual int32 GetPlayerID() const = 0;
	virtual void Turn() = 0;
	virtual void LoseCondition() = 0;
	virtual void Win() = 0;
	virtual void Reset() = 0;
};
