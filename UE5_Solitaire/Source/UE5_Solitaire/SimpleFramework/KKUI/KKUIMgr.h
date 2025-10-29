#pragma once

#include "UE5_Solitaire/SimpleFramework/KKUI/UWUIRoot.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/UWUIBase.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/UMGHelper.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/UMGAdapterTool.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
				auto mClass = LoadClass<T>(this, *path);
				if (mClass != NULL)
				{
					TWeakObjectPtr<UWUIBase> mUI = CreateWidget<T>(GetRootWidget(), mClass);
					mUI.Get()->OnCreate();
					mUIDic.Add(Key, mUI);
					return Cast<T>(mUI.Get());
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AKKUIMgr Load UI Fail: %s"), *ui_path);
				}
			}
		}

		return nullptr;
	}
	
	UWUIRoot* GetRootWidget()
	{
		if (mUIRoot == nullptr)
		{
			FString ui_path = TEXT("/Game/ResourceABs/MainScene/BPS/UI/UIRootCWBP.UIRootCWBP_C");
			auto mClass = LoadClass<UWUIRoot>(nullptr, *ui_path);
			if (mClass != nullptr)
			{
				mUIRoot = CreateWidget<UWUIRoot>(GEngine->GameViewport->GetWorld(), mClass);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AKKUIMgr Load UWUIRoot Fail: %s"), *ui_path);
			}
		}
		return mUIRoot;
	}
	
	void Init();
private:
	UPROPERTY() UWUIRoot* mUIRoot = nullptr;
	TMap<TSubclassOf<UWUIBase>, TWeakObjectPtr<UWUIBase>> mUIDic;
};