// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKObjectSingleton.generated.h"

UCLASS(Abstract)
class UE5_SOLITAIRE_API UKKObjectSingleton : public UObject
{
	GENERATED_BODY()

	static UObject* mInstance; //静态变量，特殊处理，注意一下

protected:
	template<typename T>
	static T* GetSingleton(bool bCreate = true)
	{
		static_assert(TIsDerivedFrom<T, UKKObjectSingleton>::Value, "T must be an UKKObjectSingleton derived class");
		if (mInstance == nullptr && bCreate)
		{
			mInstance = NewObject<T>(GEngine->GetWorld());
		}
		return Cast<T>(mInstance);
	}
};
