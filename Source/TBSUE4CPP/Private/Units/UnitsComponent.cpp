// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitsComponent.h"
#include "Units/Unit.h"
#include "Grid/Tile.h"
#include "Interfaces/IsPlayer.h"

// Sets default values for this component's properties
UUnitsComponent::UUnitsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PlayerInterface = Cast<IIsPlayer>(GetOwner());
	ConstructorHelpers::FObjectFinder<UClass> BP_Unit(TEXT("Class'/Game/Blueprints/BP_Unit.BP_Unit_C'"));
	if (BP_Unit.Object)
		UnitToSpawn = BP_Unit.Object;
}


// Called when the game starts
void UUnitsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UUnitsComponent::AddUnit(ATile* Tile)
{
	if (IsValid(Tile->Unit))
		return;
	UWorld* World = GetWorld();
	check(IsValid(World));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = GetOwner();
	AUnit* Unit = World->SpawnActor<AUnit>(UnitToSpawn, Tile->GetActorLocation(), FRotator::ZeroRotator, SpawnInfo);
	Unit->Initialize(Tile);
	Tile->Unit = Unit;
	Units.Add(Unit);
}

void UUnitsComponent::RemoveUnit(AUnit* UnitToRemove)
{
	Units.Remove(UnitToRemove);
	UnitToRemove->Destroy();
	check(PlayerInterface);
	if (Units.Num() == 0)
		PlayerInterface->LoseCondition();
}

void UUnitsComponent::ClearUnits()
{
	for (AUnit* Unit : Units)
	{
		Unit->GetTile()->Unit = nullptr;
		Unit->Destroy();
	}
	Units.Empty();
}

void UUnitsComponent::AddStartingUnits()
{
	for (ATile* Tile : StartingTiles)
		AddUnit(Tile);
}

void UUnitsComponent::SituationChanged() const
{
	for (AUnit* Unit : Units)
		Unit->FindReachableTiles();
}

void UUnitsComponent::NewTurn() const
{
	for (AUnit* Unit : Units)
		Unit->NewTurn();
}

ETileUnitInfo UUnitsComponent::CheckForUnits(const ATile* TileToCheck) const
{
	const AUnit* Unit = TileToCheck->Unit;
	if (!IsValid(Unit))
		return ETileUnitInfo::NoUnits;
	return Units.Contains(Unit) ? ETileUnitInfo::OwnUnit : ETileUnitInfo::EnemyUnit;
}

void UUnitsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearUnits();
}