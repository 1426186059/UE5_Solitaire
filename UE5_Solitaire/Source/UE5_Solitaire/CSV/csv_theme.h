// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class csv_theme
{
    friend class CSVConfigMgr;
public:
    struct RowData
    {
        FString RowName;
        int32 id;
        FString name;
        int32 table;
        int32 front;
        FString back;
        FString pai;
        int32 unlocktype;
        int32 unlockcost;
        int32 order;
    };
    
    TArray<RowData>& GetTable()
    {
        return mTable;
    }

private:
    TArray<RowData> mTable;

    static csv_theme* ParseData(FString csvFileContent)
    {
        csv_theme* mDataClass = new csv_theme();
        TArray<FString> Lines;
        csvFileContent.ParseIntoArrayLines(Lines);
        for (int i = 3; i < Lines.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            if (!Lines[i].IsEmpty())
            {
                RowData mRawData = ParseRowData(Lines[i]);
                mDataClass->mTable.Add(mRawData);
            }
            else
            {
                break;
            }
        }
        return mDataClass;
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
            case 0: data.RowName = Cols[i]; break;
            case 1: data.id = FCString::Atoi(*Cols[i]); break;
            case 2: data.name = Cols[i]; break;
            case 3: data.table = FCString::Atoi(*Cols[i]); break;
            case 4: data.front = FCString::Atoi(*Cols[i]); break;
            case 5: data.back = FCString::Atoi(*Cols[i]); break;
            case 6: data.pai = FCString::Atoi(*Cols[i]); break;
            case 7: data.unlocktype = FCString::Atoi(*Cols[i]); break;
            case 8: data.unlockcost = FCString::Atoi(*Cols[i]); break;
            case 9: data.order = FCString::Atoi(*Cols[i]); break;
            default: break;
            }
        }

        return data;
    }

private:
    csv_theme() = default;
    ~csv_theme() = default;
    csv_theme(const csv_theme&) = delete;
    csv_theme& operator=(const csv_theme&) = delete;
};