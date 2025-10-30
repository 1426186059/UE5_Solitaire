// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKActorSingleton.generated.h"

UCLASS(Abstract)
class UE5_SOLITAIRE_API AKKActorSingleton : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

protected:
	template<typename T>
	static T* GetActorSingleton(bool bForceCreate = true)
	{
		static_assert(TIsDerivedFrom<T, AActor>::Value, "T must be an AActor derived class");

		TSubclassOf<AActor> Key = T::StaticClass();
		TWeakObjectPtr<AActor>* mInstancePtr = mInstanceDic.Find(Key);
		if (mInstancePtr)
		{
			return Cast<T>(mInstancePtr->Get());
		}
		else
		{
			if (bForceCreate)
			{
				ensureMsgf(GEngine->GetWorld(), TEXT("GetActorSingleton<%s> GEngine->GetWorld() == null"), *Key->GetName());
				TWeakObjectPtr<AActor> mInstance = GEngine->GetWorld()->SpawnActor<T>(Key);
				mInstanceDic.Add(Key, mInstance);
				return Cast<T>(mInstance.Get());
			}
			else
			{
				return nullptr;
			}
		}

	}

private:
	static TMap<TSubclassOf<AActor>, TWeakObjectPtr<AActor>> mInstanceDic;

	void AddSingleton()
	{
		TSubclassOf<AActor> mKey = this->GetClass();
		if (!mInstanceDic.Contains(mKey))
		{
			mInstanceDic.Add(mKey, this);
		}
		else
		{
			TWeakObjectPtr<AActor>* mInstance = mInstanceDic.Find(mKey);
			if (mInstance->Get() != this)
			{
				UE_LOG(LogTemp, Error, TEXT("KKActorSingleton Error"));
			}
		}
	}

	void RemoveSingleton()
	{
		TSubclassOf<AActor> mKey = this->GetClass();
		mInstanceDic.Remove(mKey);
	}
};
