// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KKSingleton.h"

class KKTimeTool : public KKSingleton<KKTimeTool>
{
    friend class KKSingleton<KKTimeTool>;
public:

    static int64 GetTimeStamp()
    {
        return FDateTime::Now().ToUnixTimestamp();
    }


private:
    KKTimeTool(const KKTimeTool&) = delete;
    KKTimeTool& operator=(const KKTimeTool&) = delete;
    KKTimeTool(KKTimeTool&&) = delete;
    KKTimeTool& operator=(KKTimeTool&&) = delete;
};
