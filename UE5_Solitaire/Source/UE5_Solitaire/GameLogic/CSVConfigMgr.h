// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

class UE5_SOLITAIRE_API CSVConfigMgr : KKSingleton<CSVConfigMgr>
{
public:
    void LoadCSV();               // 初次读
    void ReloadCSV();             // 按 R 热重载
    const FCSVRow* FindRow(const FString& RowName) const;
private:
    TMap<FString, FCSVRow> RowMap;
    FString PakPath;            // Pak 内路径
};
