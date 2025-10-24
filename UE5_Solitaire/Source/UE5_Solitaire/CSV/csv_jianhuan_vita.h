// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class CSV_jianhuan_vita
{
    struct RowData
    {
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

private:
    static TArray<RowData> mTable;
     const int nRowOffset = 3;

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
        data.sid = Cols[0];
        data.layer = FCString::Atoi(*Cols[1]);
        data.qid = FCString::Atoi(*Cols[2]);
        data.step = FCString::Atoi(*Cols[3]);
        data.available = FCString::Atoi(*Cols[4]);
        data.jianhuanstr = Cols[5];
        data.sourcefrom = Cols[6];
        data.fromid = FCString::Atoi(*Cols[7]);
        data.sourcestr = Cols[8];
        return data;
    }

private:
    CSV_jianhuan_vita() = default;
    ~CSV_jianhuan_vita() = default;
    CSV_jianhuan_vita(const CSV_jianhuan_vita&) = delete;
    CSV_jianhuan_vita& operator=(const CSV_jianhuan_vita&) = delete;
};