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

	template<typename T>
	class Singleton
	{
		static_assert(TIsDerivedFrom<T, AKKActorSingleton>::Value, "T must be an AActor derived class");

		T* mActorInstance = nullptr;
	public:
		static Singleton<T>* GetSingleton()
		{
			static Singleton<T> Instance;
			return &Instance;
		}

		T* GetActorSingleton(bool bCreate = true)
		{
			if (this->mActorInstance == nullptr && bCreate)
			{
				ensureMsgf(GEngine->GetWorld() != nullptr, TEXT("GEngine->GetWorld() == null"));
				if (GEngine->GetWorld())
				{
					this->mActorInstance = GEngine->GetWorld()->SpawnActor<T>(T::StaticClass());
				}
			}

			T* m = Cast<T, AActor>(this->mActorInstance);
			ensureMsgf(T::mInstance, TEXT("mInstance == null"));
			ensureMsgf(m, TEXT("Cast<T>(mInstance) == null"));
			UE_LOG(LogTemp, Log, TEXT("GetSingletonInner: %s %s"), *T::StaticClass()->GetName(), *m->GetClass()->GetName());
			return m;
		}
	};

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
};
