// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class CSV_i18n
{
    struct RowData
    {
        FString key;
        FString English;
        FString Chinese;
        FString Japanese;
        FString Hindi;
        FString Arabic;
        FString French;
        FString Spanish;
        FString Russian;
    };

private:
    static TArray<RowData> mTable;

public:
    static void ParseData(FString csvFileContent)
    {
        TArray<FString> Lines;
        csvFileContent.ParseIntoArrayLines(Lines);
        for (int i = 1; i < Lines.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            RowData mRawData = ParseRowData(Lines[i]);
            mTable.Add(mRawData);
        }
    }

    static RowData ParseRowData(FString Line)
    {
        TArray<FString> Cols;
        Line.ParseIntoArray(Cols, TEXT(","));

        RowData data;
        data.key = Cols[1];
        data.English = Cols[2];
        data.Chinese = Cols[3];
        data.Japanese = Cols[4];
        data.Hindi = Cols[5];
        data.Arabic = Cols[6];
        data.French = Cols[7];
        data.Spanish = Cols[8];
        data.Russian = Cols[9];
        return data;
    }

private:
    CSV_i18n() = default;
    ~CSV_i18n() = default;
    CSV_i18n(const CSV_i18n&) = delete;
    CSV_i18n& operator=(const CSV_i18n&) = delete;
};