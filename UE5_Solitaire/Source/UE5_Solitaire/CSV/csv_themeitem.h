// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"

class csv_themeitem : public KKTypeTBase<csv_themeitem>
{
    friend class CSVConfigMgr;
public:
    TArray<FDT_themeitem>* GetTable()
    {
        return &mTable;
    }

    FDT_themeitem* GetItem(int nIndex)
    {
        return &mTable[nIndex];
    }

private:
    TArray<FDT_themeitem> mTable;

    static csv_themeitem* ParseData(FString csvFileContent)
    {
        csv_themeitem* mDataClass = new csv_themeitem();
        TArray<FString> Lines;
        csvFileContent.ParseIntoArrayLines(Lines);
        for (int i = 3; i < Lines.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            if (!Lines[i].IsEmpty())
            {
                FDT_themeitem mRawData = ParseRowData(Lines[i]);
                mDataClass->mTable.Add(mRawData);
            }
            else
            {
                break;
            }
        }
        return mDataClass;
    }

    static FDT_themeitem ParseRowData(FString Line)
    {
        TArray<FString> Cols;
        Line.ParseIntoArray(Cols, TEXT(","), false);

        FDT_themeitem data;
        for (int i = 0; i < Cols.Num(); i++)
        {
            switch (i)
            {
            case 1: data.id = FCString::Atoi(*Cols[i]); break;
            case 2: data.internalid = FCString::Atoi(*Cols[i]); break;
            case 3: data.type = FCString::Atoi(*Cols[i]); break;
            case 4: data.itemunlock = FCString::Atoi(*Cols[i]); break;
            case 5: data.thumbsrc = Cols[i]; break;
            case 6: data.download = FCString::Atoi(*Cols[i]); break;
            case 7: data.dynamic = Cols[i]; break;
            case 8: data.order = FCString::Atoi(*Cols[i]); break;
            default: break;
            }
        }

        return data;
    }

private:
    csv_themeitem() = default;
    ~csv_themeitem() = default;
    csv_themeitem(const csv_themeitem&) = delete;
    csv_themeitem& operator=(const csv_themeitem&) = delete;
};