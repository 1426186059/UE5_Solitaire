// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <typeindex>
#include <typeinfo>

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

#include "csv_i18n.h"
#include "csv_jianhuan_vita.h"

class UE5_SOLITAIRE_API CSVConfigMgr : public KKSingleton<CSVConfigMgr>
{
    friend class KKSingleton<CSVConfigMgr>;

public:
    CSV_jianhuan_vita* mCSV_jianhuan_vita;
    CSV_i18n* mCSV_i18n;
   // TMap<std:type_index, std::unique_ptr<void>> mConfigDic;
public:
    void Init();
private:
    template<typename T>
    T* LoadCSV(FString csvFileName)
    {
        FString path = FPaths::ProjectContentDir() / TEXT("/ResourceABs/MainScene/Config/CSV/") / csvFileName;

        FString CsvStr;
        if (!FFileHelper::LoadFileToString(CsvStr, *path))
        {
            UE_LOG(LogTemp, Warning, TEXT("CSV not found in Pak"));
            return nullptr;
        }
        return T::ParseData(CsvStr);
    }

    template<typename T>
    void LoadCSV2(FString csvFileName)
    {
        FString path = FPaths::ProjectContentDir() / TEXT("/ResourceABs/MainScene/Config/CSV/") / csvFileName;

        FString CsvStr;
        if (!FFileHelper::LoadFileToString(CsvStr, *path))
        {
            UE_LOG(LogTemp, Warning, TEXT("CSV not found in Pak"));
            return;
        }
        T* t = T::ParseData(CsvStr);

        std::type_index TType = std::type_index(typeid(T));
        //mConfigDic.Add(TType, t);
    }

private:
    CSVConfigMgr() = default;
    ~CSVConfigMgr() = default;
    CSVConfigMgr(const CSVConfigMgr&) = delete;
    CSVConfigMgr& operator=(const CSVConfigMgr&) = delete;
};
