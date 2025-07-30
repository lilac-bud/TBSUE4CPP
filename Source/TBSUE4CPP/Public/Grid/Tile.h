// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coordinate.h"
#include "Tile.generated.h"

UCLASS()
class TBSUE4CPP_API ATile : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMaterialInstanceDynamic* MID = nullptr;

	void CreateMID();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TileMesh;

	virtual void BeginPlay() override;

public:	
	FCoordinate Coordinate;
	int32 TileID = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATile*> Neighbours;
	class AUnit* Unit = nullptr;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor NormalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectedColor;

	ATile();
	void Initialize(const FCoordinate& CoordinateToSet, int32 TileIDToSet);
	void ChangeTileColor(bool bHighlight);
};
