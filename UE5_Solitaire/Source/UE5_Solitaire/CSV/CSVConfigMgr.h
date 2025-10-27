// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

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

public:
    TMap<FName, void*> mCSVDic;
public:
    void Init();

    template<typename T>
    T* GetCSV()
    {
        FName Key = typeid(T).name();
        return StaticCast<T*>(this->mCSVDic[Key]);
    }
private:
    template<typename T>
    void LoadCSV(FString csvFileName)
    {
        FString path = FPaths::ProjectContentDir() / TEXT("/ResourceABs/MainScene/Config/CSV/") / csvFileName;

        FString CsvStr;
        if (!FFileHelper::LoadFileToString(CsvStr, *path))
        {
            UE_LOG(LogTemp, Warning, TEXT("CSV not found in Pak"));
            return;
        }

        T* t = T::ParseData(CsvStr);
        FName Key = typeid(T).name();
        this->mCSVDic.Add(Key, t);
    }

private:
    CSVConfigMgr() = default;
    ~CSVConfigMgr() = default;
    CSVConfigMgr(const CSVConfigMgr&) = delete;
    CSVConfigMgr& operator=(const CSVConfigMgr&) = delete;
};
