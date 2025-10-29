#pragma once

#include "UE5_Solitaire/SimpleFramework/KKUI/UWUIRoot.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/UWUIBase.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameUserSettings.h"
#include "KKUIMgr.generated.h"

UCLASS()
class UE5_SOLITAIRE_API AKKUIMgr : public AKKActorSingleton
{
	GENERATED_BODY()
public:
	static AKKUIMgr* GetSingleton(bool bCreate = true)
	{
		return AKKActorSingleton::GetSingleton<AKKUIMgr>(bCreate);
	}

public:
	template<typename T>
	void Show(FString ui_path)
	{
		static_assert(TIsDerivedFrom<T, UWUIBase>::Value, "T must be an UUserWidget derived class");
		auto mUI = Get<T>(ui_path);
		if (mUI)
		{
			mUI->Show();
		}
	}

	template<typename T>
	void Hide(bool bDestory = false)
	{
		static_assert(TIsDerivedFrom<T, UWUIBase>::Value, "T must be an UUserWidget derived class");

		auto mUI = Get<T>();
		if (mUI)
		{
			mUI->Hide(bDestory);
		}
	}

	template<typename T>
	T* Get(FString ui_path = nullptr)
	{
		static_assert(TIsDerivedFrom<T, UWUIBase>::Value, "T must be an UUserWidget derived class");

		TSubclassOf<UWUIBase> Key = T::StaticClass();
		TWeakObjectPtr<UWUIBase>* mInstancePtr = mUIDic.Find(Key);
		if (mInstancePtr != nullptr)
		{
			return Cast<T>(mInstancePtr->Get());
		}
		else
		{
			bool bForceCreate = !ui_path.IsEmpty();
			if (bForceCreate)
			{
				FString path = FString::Printf(TEXT("/Game/ResourceABs/MainScene/BPS/UI/%s.%s_C"), *ui_path, *ui_path);
				auto mClass = LoadClass<T>(nullptr, path);
				if (mClass != NULL)
				{
					TWeakObjectPtr<UWUIBase> mUI = CreateWidget<T>(this, mClass);
					mUIDic.Add(Key, mInstance);
					return Cast<T>(mInstance.Get());
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AKKUIMgr Load UI Fail: %s"), *ui_path);
				}
			}
		}

		return nullptr;
	}
	
	void Init();
private:
	UWUIRoot* mUIRoot;
	TMap<TSubclassOf<UWUIBase>, TWeakObjectPtr<UWUIBase>> mUIDic;
};