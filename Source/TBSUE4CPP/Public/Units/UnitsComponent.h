// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Units/TileUnitInfo.h"
#include "UnitsComponent.generated.h"

class IIsPlayer;
class AUnit;
class ATile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBSUE4CPP_API UUnitsComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TSubclassOf<AUnit> UnitToSpawn;
	IIsPlayer* PlayerInterface;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TSet<AUnit*> Units;
	UPROPERTY()
	TArray<ATile*> StartingTiles;

	UUnitsComponent();
	void AddUnit(ATile* Tile);
	void RemoveUnit(AUnit* UnitToRemove);
	void ClearUnits();
	void AddStartingUnits();
	void SituationChanged() const;
	void NewTurn() const;
	ETileUnitInfo CheckForUnits(const ATile* TileToCheck) const;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
