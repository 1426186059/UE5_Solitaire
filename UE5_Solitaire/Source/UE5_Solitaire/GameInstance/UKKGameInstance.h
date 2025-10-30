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

	static UKKGameInstance* Get(const UObject* WorldContext = nullptr)
	{
		if (GEngine->GetWorld())
		{
			return Cast<UKKGameInstance>(GEngine->GetWorld()->GetGameInstance());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GEngine->GetWorld() == null"));
		}

		return nullptr;
	}
private:
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);
	void OnWorldCreated(UWorld* World);
};
