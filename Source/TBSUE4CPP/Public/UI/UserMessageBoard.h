// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserMessageBoard.generated.h"

/**
 * 
 */
class UTextBlock;
class UButton;

UCLASS()
class TBSUE4CPP_API UUserMessageBoard : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TurnMessage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WinMessage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* IterationsMessage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ExitToMenuButton;

public:
	void SetTurnMessage(int32 PlayerID) const;
	void SetWinMessage(int32 PlayerID) const;
	void SetIterationsMessage(int32 CurrentIteration) const;
	void RemoveWinMessage() const;
	void EnableExitToMenuButton() const;
};
