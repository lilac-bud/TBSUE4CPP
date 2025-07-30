// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/TBSPawn.h"
#include "AI/State.h"
#include "AIPawn.generated.h"

class AGrid;

UCLASS()
class TBSUE4CPP_API AAIPawn : public ATBSPawn
{
	GENERATED_BODY()

protected:
	UPROPERTY();
	class UQNetworkComponent* QNetworkComponent;
	UPROPERTY();
	AGrid* Grid = nullptr;
	UPROPERTY();
	FState CurrentState;
	UPROPERTY();
	TArray<FState> PossibleActions;
	bool bCanPlay = true;

	virtual void BeginPlay() override;
	void UpdateCurrentState();
	void GenerateActions(const class AUnit* SelectedUnit);
	struct FNetworkInput BuildNetworkInput(float Reward) const;

public:	
	AAIPawn();
	void Initialize(int32 PlayerIDToSet, AGrid* GridFromLevel);
	virtual void Turn() override;
	virtual void LoseCondition() override;
	virtual void Win() override;
	virtual void Reset() override;
};
