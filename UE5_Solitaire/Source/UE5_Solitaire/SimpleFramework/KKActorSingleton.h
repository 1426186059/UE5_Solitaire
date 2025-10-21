// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

template<typename T>
class UE5_SOLITAIRE_API KKActorSingleton : public AActor
{
public:
	KKActorSingleton()
	{
		PrimaryActorTick.bCanEverTick = true;
	}

	static T* Get(const UObject* WorldContext = nullptr)
	{
		return nullptr;
	}

protected:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
	}

	//virtual void EndPlay(EEndPlayReason::Type Reason) override
	//{
	//	Super::EndPlay(Reason);
	//}
};
