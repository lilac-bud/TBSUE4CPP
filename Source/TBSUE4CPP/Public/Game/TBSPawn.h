// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/IsPlayer.h"
#include "TBSPawn.generated.h"

UCLASS(Abstract)
class TBSUE4CPP_API ATBSPawn : public APawn, public IIsPlayer
{
	GENERATED_BODY()

protected:
	int32 PlayerID = 0;
	UPROPERTY()
	USceneComponent* SceneComponent;
	UPROPERTY();
	class UUnitsComponent* UnitsComponent;
	class ATBSGameMode* GameMode = nullptr;

	virtual void BeginPlay() override;

public:	
	ATBSPawn();
	void Initialize(int32 PlayerIDToSet);
	virtual int32 GetPlayerID() const override;
	virtual void Turn() override;
	virtual void LoseCondition() override {};
	virtual void Win() override {};
	virtual void Reset() override;
};
