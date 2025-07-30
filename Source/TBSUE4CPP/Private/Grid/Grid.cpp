// Fill out your copyright notice in the Description page of Project Settings.



#include "Grid/Grid.h"
#include "Grid/Tile.h"

AGrid::AGrid()
{
 	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	ConstructorHelpers::FObjectFinder<UClass> BP_Tile(TEXT("Class'/Game/Blueprints/BP_Tile.BP_Tile_C'"));
	if (BP_Tile.Object)
		TileToSpawn = BP_Tile.Object;
}

void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrid::ClearTiles()
{
	for (auto& Tile : Tiles)
		Tile->Destroy();
	Tiles.Empty();
}

int32 AGrid::CalculateTileIndex(const FCoordinate& Coordinate) const
{
	return Coordinate.Row * GridSize.Col + Coordinate.Col;
}

ATile* AGrid::GetNeighbourInDirection(FCoordinate Coordinate, EDirection Direction) const
{
	switch (Direction)
	{
	case EDirection::North:
		if (Coordinate.Row > 0)
			Coordinate.Row--;
		else
			return nullptr;
		break;
	case EDirection::East:
		Coordinate.Col++;
		if (Coordinate.Col == GridSize.Col)
			return nullptr;
		break;
	case EDirection::South:
		Coordinate.Row++;
		if (Coordinate.Row == GridSize.Row)
			return nullptr;
		break;
	case EDirection::West:
		if (Coordinate.Col > 0)
			Coordinate.Col--;
		else
			return nullptr;
		break;
	default:
		break;
	}
	return Tiles[CalculateTileIndex(Coordinate)];
}

void AGrid::FindTileNeighbours(ATile* Tile) const
{
	for (uint8 i = 0; i < 4; i++)
	{
		ATile* Neighbour = GetNeighbourInDirection(Tile->Coordinate, (EDirection)i);
		if (Neighbour != nullptr)
			Tile->Neighbours.Add(Neighbour);
	}
}

void AGrid::Initialize()
{
	UWorld* World = GetWorld();
	check(IsValid(World));
	ClearTiles();
	Tiles.Reserve(GridSize.Row * GridSize.Col);
	for (int RowIndex = 0; RowIndex < GridSize.Row; RowIndex++)
		for (int ColIndex = 0; ColIndex < GridSize.Col; ColIndex++)
		{
			const FVector Location = GetActorLocation();
			const FRotator Rotation = GetActorRotation();
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ATile* Tile = World->SpawnActor<ATile>(TileToSpawn, Location, Rotation, SpawnInfo);
			const FCoordinate TileCoordinate{ RowIndex,ColIndex };
			Tile->Initialize(TileCoordinate, CalculateTileIndex(TileCoordinate));
			Tiles.Add(Tile);
		}
	for (ATile* Tile : Tiles)
		FindTileNeighbours(Tile);
}