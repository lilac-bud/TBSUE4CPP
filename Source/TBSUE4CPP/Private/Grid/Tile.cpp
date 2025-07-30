// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Tile.h"

ATile::ATile()
{
 	PrimaryActorTick.bCanEverTick = false;
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
	RootComponent = TileMesh;
	TileMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::CreateMID()
{
	UMaterialInterface* Material = TileMesh->GetMaterial(0);
	MID = UMaterialInstanceDynamic::Create(Material, this);
	TileMesh->SetMaterial(0, MID);
	MID->SetVectorParameterValue(TEXT("Color"), NormalColor);
}

void ATile::Initialize(const FCoordinate& CoordinateToSet, int32 TileIDToSet)
{
	Coordinate = CoordinateToSet;
	TileID = TileIDToSet;
	if (!IsValid(GetOwner()))
		return;
	AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	const FVector& BoxExtent = TileMesh->Bounds.BoxExtent;
	SetActorRelativeLocation(FVector(BoxExtent.X * Coordinate.Col * 2, BoxExtent.Y * Coordinate.Row * 2, 0.0f));
	CreateMID();
}

void ATile::ChangeTileColor(bool bHighlight)
{
	if (IsValid(MID))
	{
		const FLinearColor& ColorToSet = bHighlight ? SelectedColor : NormalColor;
		MID->SetVectorParameterValue(TEXT("Color"), ColorToSet);
	}
}