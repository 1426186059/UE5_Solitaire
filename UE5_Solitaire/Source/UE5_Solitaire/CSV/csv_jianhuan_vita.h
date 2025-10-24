// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class CSV_jianhuan_vita
{
    struct RowData
    {
        FString id;
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
        data.id = Cols[1];
        data.layer = FCString::Atoi(*Cols[2]);
        data.qid = FCString::Atoi(*Cols[3]);
        data.step = FCString::Atoi(*Cols[4]);
        data.available = FCString::Atoi(*Cols[5]);
        data.jianhuanstr = Cols[6];
        data.sourcefrom = Cols[7];
        data.fromid = FCString::Atoi(*Cols[8]);
        data.sourcestr = Cols[9];
        return data;
    }

private:
    CSV_jianhuan_vita() = default;
    ~CSV_jianhuan_vita() = default;
    CSV_jianhuan_vita(const CSV_jianhuan_vita&) = delete;
    CSV_jianhuan_vita& operator=(const CSV_jianhuan_vita&) = delete;
};