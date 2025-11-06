// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKCSVParser.h"

class csv_i18n
{
    friend class CSVConfigMgr;
public:
    struct RowData
    {
        FString RowName;
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
    
    TArray<RowData>& GetTable()
    {
        return mTable;
    }
private:
    TArray<RowData> mTable;

    static csv_i18n* ParseData(FString csvFileContent)
    {
        csv_i18n* nDataClass = new csv_i18n();
        TArray<FString> Lines;
        csvFileContent.ParseIntoArrayLines(Lines);
        for (int i = 3; i < Lines.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            if (!Lines[i].IsEmpty())
            {
                RowData mRawData = ParseRowData(Lines[i]);
                nDataClass->mTable.Add(mRawData);
            }
            else
            {
                break;
            }
        }
        return nDataClass;
    }

    static RowData ParseRowData(FString Line)
    {
        TArray<FString> Cols;
        Line.ParseIntoArray(Cols, TEXT(","), false);

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
    csv_i18n() = default;
    ~csv_i18n() = default;
    csv_i18n(const csv_i18n&) = delete;
    csv_i18n& operator=(const csv_i18n&) = delete;
};