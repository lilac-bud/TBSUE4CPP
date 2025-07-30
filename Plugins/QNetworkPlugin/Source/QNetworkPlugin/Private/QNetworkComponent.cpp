// Fill out your copyright notice in the Description page of Project Settings.


#include "QNetworkComponent.h"
THIRD_PARTY_INCLUDES_START
#include <string>
#include <vector>
#include "Q.h"
THIRD_PARTY_INCLUDES_END
#include "NetworkInput.h"

// Sets default values for this component's properties
UQNetworkComponent::UQNetworkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UQNetworkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UQNetworkComponent::Initialize(int32 FieldHeight, int32 FieldWidth, int32 Channels, int32 PlayerID, FString Filepath)
{
	Network = new Q(FieldHeight, FieldWidth, Channels, std::to_string(PlayerID), TCHAR_TO_UTF8(*Filepath));
}

void UQNetworkComponent::SoftReset()
{
	Network->soft_reset();
}

int32 UQNetworkComponent::CallQNetwork(const FNetworkInput& NetworkInput)
{
	std::vector<float> State(NetworkInput.State.GetData(), NetworkInput.State.GetData() + NetworkInput.State.Num());
	std::vector<float> Actions(NetworkInput.Actions.GetData(), NetworkInput.Actions.GetData() + NetworkInput.Actions.Num());
	return Network->call_network(NetworkInput.Reward, State, Actions, NetworkInput.ActionsNumber);
}

void UQNetworkComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	delete Network;
}