// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KKSingleton.h"

class KKTimeTool
{
public:
    static int64 GetTimeStamp()
    {
        return FDateTime::Now().ToUnixTimestamp();
    }

    static FString GetFormatTimeStr(int64 TotalSeconds)
    {
        const int ONE_DAY = 3600 * 24;
        const int ONE_HOUR = 3600;
        const int ONE_MINUTES = 60;

        int32 Day = TotalSeconds / ONE_DAY;
        int32 Hour = (TotalSeconds - ONE_DAY * Day) / ONE_HOUR;
        int32 Minutes = (TotalSeconds - ONE_DAY * Day - Hour * ONE_HOUR) / ONE_MINUTES;
        int32 Seconds = TotalSeconds % 60;

        if (Day > 0)
        {
            return FString::Printf(TEXT("%02dd:%02d"), Day, Hour);
        }
        else if (Hour > 0)
        {
            return FString::Printf(TEXT("%02d:%02d"), Hour, Minutes);
        }
        else
        {
            return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
        }
    }

private:
    KKTimeTool() = delete;
    ~KKTimeTool() = delete;
    KKTimeTool(const KKTimeTool&) = delete;
    KKTimeTool& operator=(const KKTimeTool&) = delete;
    KKTimeTool(KKTimeTool&&) = delete;
    KKTimeTool& operator=(KKTimeTool&&) = delete;
};
