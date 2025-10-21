// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKActorSingleton.generated.h"

UCLASS()
class UE5_SOLITAIRE_API AKKActorSingleton : public AActor
{
	GENERATED_BODY()

	static AActor* mInstance;
public:
	AKKActorSingleton()
	{
		PrimaryActorTick.bCanEverTick = true;
	}

	template<typename T>
	static T* GetInstance(bool bCreate = true)
	{
		static_assert(TIsDerivedFrom<T, AActor>::Value, "T must be an AActor derived class");
		if (mInstance == nullptr && bCreate)
		{
			mInstance = GEngine->GetWorld()->SpawnActor<T>(T::StaticClass());
		}
		return mInstance;
	}
		
protected:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		if (mInstance == nullptr)
		{
			mInstance = this;
		}
		else if (mInstance != this)
		{
			UE_LOG(LogTemp, Error, TEXT("KKActorSingleton Error"));
		}
	}
	
	virtual void EndPlay(EEndPlayReason::Type Reason) override
	{
		Super::EndPlay(Reason);
		mInstance = nullptr;
	}

public:
	virtual void Tick(float DeltaTime) override
	{
		Super::Tick(DeltaTime);
	}
};
