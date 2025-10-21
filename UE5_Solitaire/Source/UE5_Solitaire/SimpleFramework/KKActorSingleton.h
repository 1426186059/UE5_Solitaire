// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameUserSettings.h"
#include "KKActorSingleton.generated.h"

// 这种方法不行
#define KKActorSingletonDefine(ClassName)												\
	UCLASS()																	\
	class ClassName##Singleton : public AActor									\
	{																			\
		GENERATED_BODY()														\
		public ClassName##Singleton()											\
		{																		\
			PrimaryActorTick.bCanEverTick = true;								\
		}																		\
		protected virtual void BeginPlay() override								\
		{																		\
			Super::BeginPlay();													\
		}																		\
		protected virtual void EndPlay(EEndPlayReason::Type Reason) override	\
		{																		\
			Super::EndPlay(Reason);												\
		}																		\
		public virtual void Tick(float DeltaTime) override						\
		{																		\
			Super::Tick(DeltaTime);												\
		}																		\
	};																			\
																				\


// 这种方法不行
template<typename T>
class KKActorSingleton : public AActor
{
	static T* Instance;
public:
	static T* GetInstance()
	{
		// 假设你只在默认世界或主世界使用
		UWorld* World = GEngine->GetWorld();
		if (World)
		{
			// 查找已存在的实例
			for (TActorIterator<T> It(World); It; ++It)
			{
				return *It;
			}
		}
		return nullptr;
	}

protected:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		if (Instance == nullptr)
		{
			Instance = this;
		}
		else if(Instance != this)
		{
			UE_LOG(LogTemp, Error, TEXT("KKActorSingleton Error"));
		}
	}

	virtual void EndPlay(EEndPlayReason::Type Reason) override
	{
		Super::EndPlay(Reason);
		Instance = nullptr;
	}

	virtual void Tick(float DeltaTime) override
	{
		Super::Tick(DeltaTime);
	}
};
