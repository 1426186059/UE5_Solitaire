// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class csv_jianhuan_newbie
{
    friend class CSVConfigMgr;
public:
    struct RowData
    {
        FString RowName;
        int32 index;
        FString sid;
    };
    
    TArray<RowData>& GetTable()
    {
        return mTable;
    }

private:
    TArray<RowData> mTable;

    static csv_jianhuan_newbie* ParseData(FString csvFileContent)
    {
        csv_jianhuan_newbie* mDataClass = new csv_jianhuan_newbie();
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
            case 1: data.index = FCString::Atoi(*Cols[i]); break;
            case 2: data.sid = FCString::Atoi(*Cols[i]); break;
            default: break;
            }
        }

        return data;
    }

private:
    csv_jianhuan_newbie() = default;
    ~csv_jianhuan_newbie() = default;
    csv_jianhuan_newbie(const csv_jianhuan_newbie&) = delete;
    csv_jianhuan_newbie& operator=(const csv_jianhuan_newbie&) = delete;
};