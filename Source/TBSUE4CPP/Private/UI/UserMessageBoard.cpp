// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserMessageBoard.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#define LOCTEXT_NAMESPACE "TBS"

void UUserMessageBoard::SetTurnMessage(int32 PlayerID) const
{
	TurnMessage->SetText(FText::Format(LOCTEXT("TurnMessage", "Player {0} turn"), PlayerID + 1));
}

void UUserMessageBoard::SetWinMessage(int32 PlayerID) const
{
	WinMessage->SetText(FText::Format(LOCTEXT("WinMessage", "Player {0} won!"), PlayerID + 1));
}

void UUserMessageBoard::SetIterationsMessage(int32 CurrentIteration) const
{
	IterationsMessage->SetText(FText::Format(LOCTEXT("IterationsMessage", "Iteration {0}"), CurrentIteration));
}

void UUserMessageBoard::RemoveWinMessage() const
{
	WinMessage->SetText(FText::GetEmpty());
}

void UUserMessageBoard::EnableExitToMenuButton() const
{
	ExitToMenuButton->SetVisibility(ESlateVisibility::Visible);
}

#undef LOCTEXT_NAMESPACE
