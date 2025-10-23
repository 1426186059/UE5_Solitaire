// Fill out your copyright notice in the Description page of Project Settings.

#include "CSVConfigMgr.h"

void CSVConfigMgr::LoadCSV()
{
    // ① 先尝试 Pak 内路径（打包/热更）
    PakPath = FPaths::ProjectContentDir() / TEXT("Data/Items.csv");
    FString CsvStr;
    if (!FFileHelper::LoadFileToString(CsvStr, *PakPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("CSV not found in Pak"));
        return;
    }

    // ② 解析
    RowMap.Reset();
    TArray<FString> Lines;
    CsvStr.ParseIntoArrayLines(Lines);
    for (int32 i = 1; i < Lines.Num(); ++i)        // 跳过表头
    {
        TArray<FString> Cols;
        Lines[i].ParseIntoArray(Cols, TEXT(","));
        if (Cols.Num() >= 4)        // 至少 4 列
        {
            FCSVRow Row;
            Row.RowName = Cols[0];
            Row.English = Cols.IsValidIndex(1) ? Cols[1] : TEXT("");
            Row.Chinese = Cols.IsValidIndex(2) ? Cols[2] : TEXT("");
            Row.Japanese = Cols.IsValidIndex(3) ? Cols[3] : TEXT("");
            RowMap.Add(Row.RowName, Row);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("CSV loaded: %d rows"), RowMap.Num());
}

void CSVConfigMgr::ReloadCSV()
{
    LoadCSV();
    UE_LOG(LogTemp, Log, TEXT("CSV reloaded"));
}

const FCSVRow* CSVConfigMgr::FindRow(const FString& RowName) const
{
    return RowMap.Find(RowName);
}
