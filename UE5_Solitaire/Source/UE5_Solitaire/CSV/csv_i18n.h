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
        for (int i = 3; i < Lines.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            if (!Lines[i].IsEmpty())
            {
                RowData mRawData = ParseRowData(Lines[i]);
                mTable.Add(mRawData);
            }
            else
            {
                break;
            }
        }
    }

    static RowData ParseRowData(FString Line)
    {
        TArray<FString> Cols;
        Line.ParseIntoArray(Cols, TEXT(","));
        ensure(Cols.Num() == 9, "Cols Num: " + Cols.Num());

        RowData data;
        data.key = Cols[0];
        data.English = Cols[1];
        data.Chinese = Cols[2];
        data.Japanese = Cols[3];
        data.Hindi = Cols[4];
        data.Arabic = Cols[5];
        data.French = Cols[6];
        data.Spanish = Cols[7];
        data.Russian = Cols[8];
        return data;
    }

private:
    CSV_i18n() = default;
    ~CSV_i18n() = default;
    CSV_i18n(const CSV_i18n&) = delete;
    CSV_i18n& operator=(const CSV_i18n&) = delete;
};