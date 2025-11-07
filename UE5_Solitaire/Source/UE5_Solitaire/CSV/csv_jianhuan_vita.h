// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"

class csv_jianhuan_vita : public KKTypeTBase<csv_jianhuan_vita>
{
    friend class CSVConfigMgr;
public:
    TArray<FDT_jianhuan_vita>* GetTable()
    {
        return &mTable;
    }

    FDT_jianhuan_vita* GetItem(int nIndex)
    {
        return &mTable[nIndex];
    }

private:
    TArray<FDT_jianhuan_vita> mTable;

    static csv_jianhuan_vita* ParseData(FString csvFileContent)
    {
        csv_jianhuan_vita* mDataClass = new csv_jianhuan_vita();

        TArray<TArray<FString>> mOriTable = KKCSVParser::ParseFileContent(csvFileContent);
        for (int i = 3; i < mOriTable.Num(); ++i)        // Ìø¹ý±íÍ·
        {
            FDT_jianhuan_vita mRawData = ParseRowData(mOriTable[i]);
            mDataClass->mTable.Add(mRawData);
        }
        return mDataClass;
    }

    static FDT_jianhuan_vita ParseRowData(TArray<FString> Cols)
    {
        FDT_jianhuan_vita data;
        for (int i = 0; i < Cols.Num(); i++)
        {
            switch (i)
            {
            case 1: data.sid = Cols[i]; break;
            case 2: data.layer = FCString::Atoi(*Cols[i]); break;
            case 3: data.qid = FCString::Atoi(*Cols[i]); break;
            case 4: data.step = FCString::Atoi(*Cols[i]); break;
            case 5: data.available = FCString::Atoi(*Cols[i]); break;
            case 6: data.jianhuanstr = Cols[i]; break;
            case 7: data.sourcefrom = Cols[i]; break;
            case 8: data.fromid = FCString::Atoi(*Cols[i]); break;
            case 9: data.sourcestr = Cols[i]; break;
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