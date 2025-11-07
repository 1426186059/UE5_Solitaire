// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"

#include "csv_i18n.h"
#include "csv_jianhuan_newbie.h"
#include "csv_jianhuan_vita.h"
#include "csv_stagereward.h"
#include "csv_table2element.h"
#include "csv_theme.h"
#include "csv_themeitem.h"


class UE5_SOLITAIRE_API CSVConfigMgr : public KKSingleton<CSVConfigMgr>
{
    friend class KKSingleton<CSVConfigMgr>;

private:
    TMap<uint64, void*> mCSVDic;
public:
    void Init();

    template<typename T>
    T* GetCSV()
    {
        static_assert(TIsDerivedFrom<T, KKTypeTBase<T>>::Value, "T must be an KKTypeTBase derived class");
        uint64 Key = T::GetTypeId();
        return (T*)(this->mCSVDic[Key]);
    }
private:
    template<typename T>
    void LoadCSV(FString csvFileName)
    {
        static_assert(TIsDerivedFrom<T, KKTypeTBase<T>>::Value, "T must be an KKTypeTBase derived class");

        FString path = FPaths::ProjectContentDir() / TEXT("ResourceABs/MainScene/Config/CSV/") / csvFileName;

        FString CsvStr;
        if (!FFileHelper::LoadFileToString(CsvStr, *path))
        {
            UE_LOG(LogTemp, Error, TEXT("LoadCSV Error+: %s"), *path);
            return;
        }

        T* t = T::ParseData(CsvStr);
        uint64 Key = T::GetTypeId();
        this->mCSVDic.Add(Key, t);
    }

private:
    CSVConfigMgr() = default;
    ~CSVConfigMgr() = default;
    CSVConfigMgr(const CSVConfigMgr&) = delete;
    CSVConfigMgr& operator=(const CSVConfigMgr&) = delete;
};
