// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/TBSPawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class TBSUE4CPP_API APlayerPawn : public ATBSPawn
{
	GENERATED_BODY()

private:
	APlayerController* PlayerController = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	APlayerPawn();
	void Initialize(int32 PlayerIDToSet);
	virtual void Turn() override;
	virtual void LoseCondition() override;
	virtual void Win() override;
};
