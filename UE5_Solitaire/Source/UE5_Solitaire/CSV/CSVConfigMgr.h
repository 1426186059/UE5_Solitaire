// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <typeindex>

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

#include "csv_i18n.h"
#include "csv_jianhuan_vita.h"

inline uint32 GetTypeHash(const std::type_index& Idx)
{
    return static_cast<uint32>(Idx.hash_code());
}

class UE5_SOLITAIRE_API CSVConfigMgr : public KKSingleton<CSVConfigMgr>
{
    friend class KKSingleton<CSVConfigMgr>;

public:
    CSV_jianhuan_vita* mCSV_jianhuan_vita;
    CSV_i18n* mCSV_i18n;
    TMap<FName, TUniquePtr<void>> mCSVDic;
public:
    void Init();
    template<typename T>
    T* GetCSV()
    {
        FName Key = typeid(T).name();
        return StaticCast<T*>(this->mCSVDic[Key].Get());
    }
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
        FName Key = typeid(T).name();
        this->mCSVDic[Key] = t;
    }

private:
    CSVConfigMgr() = default;
    ~CSVConfigMgr() = default;
    CSVConfigMgr(const CSVConfigMgr&) = delete;
    CSVConfigMgr& operator=(const CSVConfigMgr&) = delete;
};
