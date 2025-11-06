#pragma once
#include "CoreMinimal.h"

class KKCSVParser
{
public:
    static TArray<TArray<FString>> ParseFileContent(FString csvFileContent)
    {
        TArray<TArray<FString>> mTable = {};

        TArray<FString> Lines;
        csvFileContent.ParseIntoArrayLines(Lines);
        for (int i = 3; i < Lines.Num(); ++i)        // 跳过表头
        {
            if (!Lines[i].IsEmpty())
            {
                TArray<FString> mArray = ParseRow(Lines[i]);
                mTable.Add(mArray);
            }
            else
            {
                break;
            }
        }

        return mTable;
    }
    
    static TArray<FString> ParseRow(FString Line)
    {
        TArray<FString> Cols;
        Line.ParseIntoArray(Cols, TEXT(","), false);

        TArray<FString> Cols2;
        FString traceEnd;
        FString CombineContent = "";
        for (int i = 0; i < Cols.Num(); i++)
        {
            if (traceEnd.IsEmpty())
            {
                if (Cols[i].Contains("\""))
                {
                    traceEnd = "\"";
                }
                else if (Cols[i].Contains("["))
                {
                    traceEnd = "]";
                }
                else if (Cols[i].Contains("{"))
                {
                    traceEnd = "}";
                }
                else
                {
                    Cols2.Add(Cols[i]);
                }

                if (!traceEnd.IsEmpty())
                {
                    CombineContent += Cols[i] + ", ";
                }
            }
            else
            {
                if (Cols[i].Contains(traceEnd))
                {
                    CombineContent += Cols[i];
                    Cols2.Add(CombineContent);
                    traceEnd = "";
                    CombineContent = "";
                }
                else
                {
                    CombineContent += Cols[i] + ", ";
                }
            }
        }

        return Cols2;
    }
    
private:
    // 禁止拷贝和移动（单例不应被复制）
    KKCSVParser() = delete;
    ~KKCSVParser() = delete;
    KKCSVParser(const KKCSVParser&) = delete;
    KKCSVParser& operator=(const KKCSVParser&) = delete;
    KKCSVParser(KKCSVParser&&) = delete;
    KKCSVParser& operator=(KKCSVParser&&) = delete;
};
