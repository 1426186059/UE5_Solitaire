// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class csv_stagereward
{
    friend class CSVConfigMgr;
public:
    TArray<FDT_stagereward>& GetTable()
    {
        return mTable;
    }

private:
    TArray<FDT_stagereward> mTable;

    static csv_stagereward* ParseData(FString csvFileContent)
    {
        csv_stagereward* mDataClass = new csv_stagereward();
        TArray<FString> Lines;
        csvFileContent.ParseIntoArrayLines(Lines);
        for (int i = 3; i < Lines.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            if (!Lines[i].IsEmpty())
            {
                FDT_stagereward mRawData = ParseRowData(Lines[i]);
                mDataClass->mTable.Add(mRawData);
            }
            else
            {
                break;
            }
        }
        return mDataClass;
    }

    static FDT_stagereward ParseRowData(FString Line)
    {
        TArray<FString> Cols;
        Line.ParseIntoArray(Cols, TEXT(","), false);

        FDT_stagereward data;
        for (int i = 0; i < Cols.Num(); i++)
        {
            switch (i)
            {
            case 1: data.id = FCString::Atoi(*Cols[i]); break;
            case 2: data.rewardtype = FCString::Atoi(*Cols[i]); break;
            case 3: data.rewardid = FCString::Atoi(*Cols[i]); break;
            case 4: data.wincount = FCString::Atoi(*Cols[i]); break;
            case 5: data.imgsrc = Cols[i]; break;
            default: break;
            }
        }

        return data;
    }

private:
    csv_stagereward() = default;
    ~csv_stagereward() = default;
    csv_stagereward(const csv_stagereward&) = delete;
    csv_stagereward& operator=(const csv_stagereward&) = delete;
};