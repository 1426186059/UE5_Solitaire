// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class csv_table2element
{
    friend class CSVConfigMgr;
public:
    TArray<FDT_table2element>& GetTable()
    {
        return mTable;
    }

private:
    TArray<FDT_table2element> mTable;

    static csv_table2element* ParseData(FString csvFileContent)
    {
        csv_table2element* mDataClass = new csv_table2element();
        TArray<FString> Lines;
        csvFileContent.ParseIntoArrayLines(Lines);
        for (int i = 3; i < Lines.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            if (!Lines[i].IsEmpty())
            {
                FDT_table2element mRawData = ParseRowData(Lines[i]);
                mDataClass->mTable.Add(mRawData);
            }
            else
            {
                break;
            }
        }
        return mDataClass;
    }

    static FDT_table2element ParseRowData(FString Line)
    {
        TArray<FString> Cols;
        Line.ParseIntoArray(Cols, TEXT(","), false);

        FDT_table2element data;
        for (int i = 0; i < Cols.Num(); i++)
        {
            switch (i)
            {
            case 1: data.id = FCString::Atoi(*Cols[i]); break;
            case 2: data.tableid = FCString::Atoi(*Cols[i]); break;
            case 3: data.paiid = FCString::Atoi(*Cols[i]); break;
            default: break;
            }
        }

        return data;
    }

private:
    csv_table2element() = default;
    ~csv_table2element() = default;
    csv_table2element(const csv_table2element&) = delete;
    csv_table2element& operator=(const csv_table2element&) = delete;
};