// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"

#include "dt_i18n.h"
#include "dt_jianhuan_vita.h"

class UE5_SOLITAIRE_API DTMgr : public KKSingleton<DTMgr>
{
    friend class KKSingleton<DTMgr>;
public:
    void Init();

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
        static_assert(TIsDerivedFrom<T, UDataTable>::Value, "T must be an UDataTable derived class");
        T* mTable = LoadObject<T>(nullptr, *path);
        TSubclassOf<UDataTable> mKey = T::StaticClass();
        ensure(mTable);
        mConfigDic.Add(mKey, mTable);
    }

    static TMap<TSubclassOf<UDataTable>, TWeakObjectPtr<UDataTable>> mConfigDic;

private:
    DTMgr() = default;
    ~DTMgr() = default;
    DTMgr(const DTMgr&) = delete;
    DTMgr& operator=(const DTMgr&) = delete;
};
