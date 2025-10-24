// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

#include "csv_i18n.h"
#include "csv_jianhuan_vita.h"

class UE5_SOLITAIRE_API CSVConfigMgr : public KKSingleton<CSVConfigMgr>
{
    friend class KKSingleton<CSVConfigMgr>;
public:
    static const int RowOffset = 3;
public:
    void Init();
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

        T::ParseData(CsvStr);
    }

private:
    CSVConfigMgr() = default;
    ~CSVConfigMgr() = default;
    CSVConfigMgr(const CSVConfigMgr&) = delete;
    CSVConfigMgr& operator=(const CSVConfigMgr&) = delete;
};
