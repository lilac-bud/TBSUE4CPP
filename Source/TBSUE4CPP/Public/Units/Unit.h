// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"

class ATile;
class UUnitsComponent;

UCLASS()
class TBSUE4CPP_API AUnit : public AActor
{
	GENERATED_BODY()
	
private:
	const int32 MaxMovementPoints = 5;
	UUnitsComponent* UnitsComponent = nullptr;
	class UHealthWidget* HealthWidget = nullptr;

	void Move(ATile* TargetTile);
	void Attack(ATile* TargetTile);
	void PassOverRing(const int32 CurrentCost, TArray<ATile*>& CurrentRing);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* UnitMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HealthWidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Health = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATile* Tile = nullptr;
	TMap<ATile*, int32> ReachableTilesCosts;
	int32 MovementPoints = MaxMovementPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLinearColor> UnitColors;

	virtual void BeginPlay() override;

public:
	const float AttackDamage = 0.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATile*> ReachableTiles;

	AUnit();
	float GetHealth() const;
	ATile* GetTile() const;
	ATile* FindCloserTile(ATile* TargetTile) const;
	void Initialize(ATile* TileToSet);
	void GetHit(float Damage);
	void Die();
	void Act(ATile* TargetTile);
	void FindReachableTiles();
	void NewTurn();
};
