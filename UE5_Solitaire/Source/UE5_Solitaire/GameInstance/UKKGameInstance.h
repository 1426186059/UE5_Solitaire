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

	static UKKGameInstance* Get(const UObject* WorldContext = nullptr)
	{
		return mInstance;
	}

	static UKKGameInstance* GetWorld()
	{
		return mInstance->GetWorld();
	}

private:
	static UKKGameInstance* mInstance;
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);
	void OnWorldCreated(UWorld* World);
};
