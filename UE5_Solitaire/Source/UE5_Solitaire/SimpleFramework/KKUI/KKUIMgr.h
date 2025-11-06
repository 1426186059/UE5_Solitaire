#pragma once

#include "UE5_Solitaire/SimpleFramework/KKUI/UWUIRoot.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/UWUIBase.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "Components/WidgetComponent.h"

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
		return AKKActorSingleton::GetActorSingleton<AKKUIMgr>(bCreate);
	}

public:
	template<typename T>
	void Show(FString ui_path = TEXT(""))
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
	T* Get(FString ui_path = TEXT(""))
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
			mUIDic.Remove(Key);
			bool bForceCreate = !ui_path.IsEmpty();
			if (bForceCreate)
			{
				FString path = FString::Printf(TEXT("/Game/ResourceABs/MainScene/BPS/UI/%s.%s_C"), *ui_path, *ui_path);
				T* t = Load<T>(path);
				if (t != nullptr)
				{
					t->OnCreate();

					TWeakObjectPtr<UWUIBase> mUI = TWeakObjectPtr<UWUIBase>(t);
					mUIDic.Add(Key, mUI);
					return t;
				}
			}
		}

		return nullptr;
	}

	template<typename T>
	T* Load(FString ui_path)
	{
		static_assert(TIsDerivedFrom<T, UWUIBase>::Value, "T must be an UUserWidget derived class");

		if (!ui_path.EndsWith("_C"))
		{
			ui_path += FString("_C");
		}

		auto mClass = LoadClass<T>(this, *ui_path);
		if (mClass != NULL)
		{
			return CreateWidget<T>(GetRootWidget(), mClass);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AKKUIMgr Load UI Fail: %s"), *ui_path);
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
				mUIRoot = CreateWidget<UWUIRoot>(UEHelper::GetKKWorld(), mClass);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AKKUIMgr Load UWUIRoot Fail: %s"), *ui_path);
			}
		}
		return mUIRoot;
	}

	template<typename WidgetT = UUserWidget>
	WidgetT* KKCreateWidget(TSubclassOf<UUserWidget> mClass)
	{
		static_assert(TIsDerivedFrom<WidgetT, UUserWidget>::Value, "T must be an UWidget derived class");
		return CreateWidget<WidgetT>(GetRootWidget(), mClass);
	}
	
	void Init();
private:
	UPROPERTY() UWUIRoot* mUIRoot = nullptr;
	UWidgetComponent* mWidgetComponent = nullptr;
	TMap<TSubclassOf<UWUIBase>, TWeakObjectPtr<UWUIBase>> mUIDic;
};