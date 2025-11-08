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

template<typename T>
class UDataTableTMgr : KKTypeTBase<UDataTableTMgr<T>>
{
private:
    UDataTable* mTable;
    TArray<T*> mTableT = {};
public:
    UDataTableTMgr(UDataTable* t)
    {
        static_assert(TIsDerivedFrom<T, FTableRowBase>::Value, "T must be an FTableRowBase derived class");
        this->mTable = t;
    }

    const TArray<T*>* GetTableT()
    {
        if (mTableT.Num() == 0)
        {
            mTable->GetAllRows("", mTableT);
        }
        return &mTableT;
    }
};

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
    UDataTableTMgr<T>* Get()
    {
        static_assert(TIsDerivedFrom<T, FTableRowBase>::Value, "T must be an FTableRowBase derived class");

        FString mKey = T::StaticStruct()->GetFName().ToString();
        void** mPtr = mConfigDic.Find(mKey);
        if (mPtr)
        {
            return (UDataTableTMgr<T>*)(*mPtr);
        }

        UE_LOG(LogTemp, Error, TEXT("ADTMgr Get Error: %s"), *mKey);
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

        static_assert(TIsDerivedFrom<T, FTableRowBase>::Value, "T must be an FTableRowBase derived class");
        UDataTable* mTable = LoadObject<UDataTable>(nullptr, *path);
        FString mKey = T::StaticStruct()->GetFName().ToString();
        ensure(mTable);
        mConfigDic.Add(mKey, new UDataTableTMgr<T>(mTable));
    }

    TMap<FString, void*> mConfigDic;
};
