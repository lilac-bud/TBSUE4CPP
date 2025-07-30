// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBSPlayerController.generated.h"

class ATile;
class AUnit;

UCLASS()
class TBSUE4CPP_API ATBSPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	class ATBSGameMode* GameMode = nullptr;
	class UUnitsComponent* UnitsComponent = nullptr;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATile* SelectedTile = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AUnit* SelectedUnit = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATile*> HighlightedTiles;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanPlay = false;

	ATile* GetClickedTile() const;

public:
	ATBSPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	void Click();
	void EndTurn();
};
