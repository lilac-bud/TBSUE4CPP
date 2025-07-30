// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/Coordinate.h"
#include "Grid.generated.h"

enum class EDirection : uint8 { North, East, South, West };

class ATile;

UCLASS()
class TBSUE4CPP_API AGrid : public AActor
{
	GENERATED_BODY()

private:
	TSubclassOf<ATile> TileToSpawn;

	void ClearTiles();
	int32 CalculateTileIndex(const FCoordinate& Coordinate) const;
	ATile* GetNeighbourInDirection(FCoordinate Coordinate, EDirection Direction) const;
	void FindTileNeighbours(ATile* Tile) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCoordinate GridSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATile*> Tiles;

	AGrid();
	UFUNCTION(CallInEditor)
	void Initialize();
};
