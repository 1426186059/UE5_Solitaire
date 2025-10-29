// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKActorSingleton.generated.h"

UCLASS(Abstract)
class UE5_SOLITAIRE_API AKKActorSingleton : public AActor
{
	GENERATED_BODY()

	static AActor* mInstance; //静态变量，特殊处理，注意一下
protected:
	template<typename T>
	static T* GetSingleton(bool bCreate = true)
	{
		static_assert(TIsDerivedFrom<T, AKKActorSingleton>::Value, "T must be an AActor derived class");
		if (mInstance == nullptr && bCreate)
		{
			mInstance = GEngine->GameViewport->GetWorld()->SpawnActor<T>(T::StaticClass());
			ensureMsgf(mInstance != nullptr, TEXT("GetSingleton() == null"));
			ensureMsgf(Cast<T>(mInstance) != nullptr, TEXT("GetSingleton() == null"));
		}
		return Cast<T>(mInstance);
	}

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
};
