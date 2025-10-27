// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class csv_jianhuan_vita
{
    friend class CSVConfigMgr;
public:
    struct RowData
    {
        FString RowName;
        FString sid;
        int32 layer;
        int32 qid;
        int32 step;
        int32 available;
        FString jianhuanstr;
        FString sourcefrom;
        int32 fromid;
        FString sourcestr;
    };
    
    TArray<RowData>& GetTable()
    {
        return mTable;
    }

private:
    TArray<RowData> mTable;

    static csv_jianhuan_vita* ParseData(FString csvFileContent)
    {
        csv_jianhuan_vita* mDataClass = new csv_jianhuan_vita();
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
            case 0: data.sid = Cols[i]; break;
            case 1: data.layer = FCString::Atoi(*Cols[i]); break;
            case 2: data.qid = FCString::Atoi(*Cols[i]); break;
            case 3: data.step = FCString::Atoi(*Cols[i]); break;
            case 4: data.available = FCString::Atoi(*Cols[i]); break;
            case 5: data.jianhuanstr = Cols[i]; break;
            case 6: data.sourcefrom = Cols[i]; break;
            case 7: data.fromid = FCString::Atoi(*Cols[i]); break;
            case 8: data.sourcestr = Cols[i]; break;
            default: break;
            }
        }

        return data;
    }

private:
    csv_jianhuan_vita() = default;
    ~csv_jianhuan_vita() = default;
    csv_jianhuan_vita(const csv_jianhuan_vita&) = delete;
    csv_jianhuan_vita& operator=(const csv_jianhuan_vita&) = delete;
};