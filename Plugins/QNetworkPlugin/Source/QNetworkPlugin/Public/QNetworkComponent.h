// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QNetworkComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QNETWORKPLUGIN_API UQNetworkComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	class Q* Network;

protected:
	virtual void BeginPlay() override;

public:	
	UQNetworkComponent();
	
	void Initialize(int32 FieldHeight, int32 FieldWidth, int32 Channels, int32 PlayerID, FString Filepath);
	void SoftReset();
	int32 CallQNetwork(const struct FNetworkInput& NetworkInput);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
