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
		return Cast<UKKGameInstance>(GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)->GetGameInstance());
	}
	
	template<typename T>
	T* GetActorSingleton(bool bForceCreate = true)
	{
		static_assert(TIsDerivedFrom<T, AActor>::Value, "T must be an AActor derived class");

		TSubclassOf<AActor> Key = T::StaticClass();
		TWeakObjectPtr<AActor>* mInstancePtr = mapActorSingleTon.Find(Key);
		if (mInstancePtr != nullptr)
		{
			return Cast<T>(mInstancePtr->Get());
		}
		else
		{
			if (bForceCreate)
			{
				TWeakObjectPtr<AActor> mInstance = GetWorld()->SpawnActor<T>(Key);
				mapActorSingleTon.Add(Key, mInstance);
				return Cast<T>(mInstance.Get());
			}
			else
			{
				return nullptr;
			}
		}
	}

	TMap<TSubclassOf<AActor>, TWeakObjectPtr<AActor>> mapActorSingleTon;

public:
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);
};
