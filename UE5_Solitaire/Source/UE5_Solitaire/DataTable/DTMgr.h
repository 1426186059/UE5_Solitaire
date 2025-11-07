// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"

#include "dt_jianhuan_newbie.h"
#include "dt_stagereward.h"
#include "dt_table2element.h"
#include "dt_theme.h"
#include "dt_themeitem.h"
#include "dt_i18n.h"
#include "dt_jianhuan_vita.h"

#include "DTMgr.generated.h"

UCLASS()
class UE5_SOLITAIRE_API ADTMgr : public AKKActorSingleton
{
    GENERATED_BODY()
public:
    void Init();

    static ADTMgr* GetSingleton(bool bCreate = true)
    {
        return ADTMgr::GetActorSingleton<ADTMgr>(bCreate);
    }

    template<typename T>
    T* Get()
    {
        static_assert(TIsDerivedFrom<T, UDataTable>::Value, "T must be an UDataTable derived class");

        TSubclassOf<UDataTable> Key = T::StaticClass();
        TWeakObjectPtr<UDataTable>* mPtr = mConfigDic.Find(Key);
        if (mPtr)
        {
            return Cast<T>(mPtr->Get());
        }

        ensure(mPtr);
        return nullptr;
    }

private:
    template<typename T>
    void LoadTable(FString path)
    {
        if (!path.StartsWith(TEXT("/Game/")))
        {
            FString fileName = path;
            path = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Config/DT/%s.%s"), *fileName, *fileName);
        }

        static_assert(TIsDerivedFrom<T, UDataTable>::Value, "T must be an UDataTable derived class");
        UDataTable* mTable = LoadObject<UDataTable>(nullptr, *path);
        TSubclassOf<UDataTable> mKey = T::StaticClass();
        ensure(mTable);
        mConfigDic.Add(mKey, mTable);
    }

    TMap<TSubclassOf<UDataTable>, TWeakObjectPtr<UDataTable>> mConfigDic;
};
