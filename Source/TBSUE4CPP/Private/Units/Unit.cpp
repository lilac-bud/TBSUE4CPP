// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/Unit.h"
#include "Units/UnitsComponent.h"
#include "Units/TileUnitInfo.h"
#include "Grid/Tile.h"
#include "UI/HealthWidget.h"
#include "Interfaces/IsPlayer.h"
#include "Components/WidgetComponent.h"

AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	UnitMesh = CreateDefaultSubobject<UStaticMeshComponent>("UnitMesh");
	RootComponent = SceneComponent;
	UnitMesh->SetupAttachment(SceneComponent);
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComponent->SetupAttachment(UnitMesh);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

}

float AUnit::GetHealth() const
{
	return Health;
}

ATile* AUnit::GetTile() const
{
	return Tile;
}

void AUnit::Initialize(ATile* TileToSet)
{
	Tile = TileToSet;
	HealthWidget = Cast<UHealthWidget>(HealthWidgetComponent->GetUserWidgetObject());
	check(IsValid(GetOwner()));
	UnitsComponent = (UUnitsComponent*)GetOwner()->GetComponentByClass(UUnitsComponent::StaticClass());
	if (UnitColors.Num() == 0) return;
	UMaterialInterface* Material = UnitMesh->GetMaterial(0);
	UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(Material, this);
	UnitMesh->SetMaterial(0, MID);
	IIsPlayer* OwnerInterface = Cast<IIsPlayer>(GetOwner());
	if (OwnerInterface)
		MID->SetVectorParameterValue(TEXT("Color"), UnitColors[OwnerInterface->GetPlayerID()]);
}

void AUnit::GetHit(float Damage)
{
	Health -= Damage;
	if (Health <= 0.0f)
		Die();
	else if (HealthWidget)
		HealthWidget->UpdateHealthBar(Health);
}

void AUnit::Die()
{
	Tile = nullptr;
	check(IsValid(UnitsComponent));
	UnitsComponent->RemoveUnit(this);
}

void AUnit::Move(ATile* TargetTile)
{
	MovementPoints -= ReachableTilesCosts[TargetTile];
	Tile->Unit = nullptr;
	Tile = TargetTile;
	TargetTile->Unit = this;
	SetActorLocation(TargetTile->GetActorLocation());
	check(IsValid(UnitsComponent));
	UnitsComponent->SituationChanged();
}

void AUnit::Attack(ATile* TargetTile)
{
	if (!Tile->Neighbours.Contains(TargetTile))
		Move(FindCloserTile(TargetTile));
	TargetTile->Unit->GetHit(AttackDamage);
	if (IsValid(TargetTile->Unit))
		MovementPoints = 0;
	else
		Move(TargetTile);
}

ATile* AUnit::FindCloserTile(ATile* TargetTile) const
{
	ATile* CurrentClosestTile = TargetTile;
	int32 CurrentCost = ReachableTilesCosts[TargetTile];
	for (ATile* NeighbourTile : TargetTile->Neighbours)
	{
		if (IsValid(NeighbourTile->Unit))
			continue;
		const int32* Cost = ReachableTilesCosts.Find(NeighbourTile);
		if (Cost && *Cost <= CurrentCost)
		{
			CurrentClosestTile = NeighbourTile;
			CurrentCost = *Cost;
		}
	}
	return CurrentClosestTile;
}

void AUnit::Act(ATile* TargetTile)
{
	if (ReachableTiles.Contains(TargetTile))
		IsValid(TargetTile->Unit) ? Attack(TargetTile) : Move(TargetTile);
}

void AUnit::PassOverRing(const int32 CurrentCost, TArray<ATile*>& CurrentRing)
{
	TArray<ATile*> NewRing;
	for (const ATile* TileInCurrentRing : CurrentRing)
		for (ATile* CurrentTile : TileInCurrentRing->Neighbours)
			if (!ReachableTilesCosts.Contains(CurrentTile) && UnitsComponent)
				switch (UnitsComponent->CheckForUnits(CurrentTile))
				{
				case ETileUnitInfo::OwnUnit:
					break;
				case ETileUnitInfo::NoUnits:
					if (CurrentCost < MovementPoints)
						NewRing.Add(CurrentTile);
					//pass to the EnemyUnit case
				case ETileUnitInfo::EnemyUnit:
					ReachableTilesCosts.Add(CurrentTile, CurrentCost);
					break;
				default:
					break;
				}
	CurrentRing = NewRing;
}

void AUnit::FindReachableTiles()
{
	ReachableTiles.Empty();
	ReachableTilesCosts.Empty();
	ReachableTilesCosts.Add(Tile, 0);
	TArray<ATile*> CurrentRing = { Tile };
	for (int32 CurrentCost = 1; CurrentCost <= MovementPoints; CurrentCost++)
		PassOverRing(CurrentCost, CurrentRing);
	ReachableTilesCosts.GenerateKeyArray(ReachableTiles);
}

void AUnit::NewTurn()
{
	MovementPoints = MaxMovementPoints;
	FindReachableTiles();
}