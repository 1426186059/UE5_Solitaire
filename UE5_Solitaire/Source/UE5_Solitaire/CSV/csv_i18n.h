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

        RowData data;
        for (int i = 0; i < Cols.Num(); i++)
        {
            switch (i)
            {
            case 0: data.key = Cols[i]; break;
            case 1: data.English = Cols[i]; break;
            case 2: data.Chinese = Cols[i]; break;
            case 3: data.Japanese = Cols[i]; break;
            case 4: data.Hindi = Cols[i]; break;
            case 5: data.Arabic = Cols[i]; break;
            case 6: data.French = Cols[i]; break;
            case 7: data.Spanish = Cols[i]; break;
            case 8: data.Russian = Cols[i]; break;
            default: break;
            }
        }
        return data;
    }

private:
    CSV_i18n() = default;
    ~CSV_i18n() = default;
    CSV_i18n(const CSV_i18n&) = delete;
    CSV_i18n& operator=(const CSV_i18n&) = delete;
};