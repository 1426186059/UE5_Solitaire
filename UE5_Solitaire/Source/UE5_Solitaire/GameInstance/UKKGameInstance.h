// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/GameLogic/GameLauncher.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UKKGameInstance.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UKKGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnStart() override;

	static UKKGameInstance* Get()
	{
		return mInstance;
	}

	static UWorld* GetKKWorld()
	{
		return mInstance->GetWorld();
	}

private:
	static UKKGameInstance* mInstance;
	void CreateGameLauncher();
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);
	void OnWorldCreated(UWorld* World);
};
