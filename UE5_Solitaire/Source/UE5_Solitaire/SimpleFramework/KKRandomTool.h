// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <random>
#include "CoreMinimal.h"

class KKRandomTool
{
public:
    static int32 RandomInt(int32 min, int32 max)
    {
        return FMath::RandRange(min, max);
    }

    static int32 RandomArrayInt(int32 nArrayCount)
    {
        return FMath::RandRange(0, nArrayCount - 1);
    }

    static int32 GetIndexByRate(const TArray<int32>& tableRate)
    {
        int32 nSumRate = 0;
        for(int32 v : tableRate)
        {
            nSumRate += v;
        }

        int32 nTempTargetRate = nSumRate + 1;
        if (nSumRate >= 1)
        {
            nTempTargetRate = RandomInt(1, nSumRate);
        }
        
        int32 nTempRate = 0;
        int32 nTargetIndex = -1;
        for (int32 i = 0; i < tableRate.Num(); i++)
        {
            nTempRate += tableRate[i];
            if (nTempRate >= nTempTargetRate)
            {
                nTargetIndex = i;
                break;
            }
        }

        return nTargetIndex;
    }

private:
    KKRandomTool() = delete;
    ~KKRandomTool() = delete;
    KKRandomTool(const KKRandomTool&) = delete;
    KKRandomTool& operator=(const KKRandomTool&) = delete;
    KKRandomTool(KKRandomTool&&) = delete;
    KKRandomTool& operator=(KKRandomTool&&) = delete;
};

class KKRandom
{
public:
    KKRandom(int64_t Seed)
    {
        mRandom = std::mt19937(Seed); //Mersenne Twister ÒýÇæ
    }
    
    int32 RandomInt(int32 min, int32 max)
    {
        std::uniform_int_distribution<int32> dist(min, max);
        int32 random_number = dist(mRandom);
        return random_number;
    }

private:
    std::mt19937 mRandom;
    KKRandom(const KKRandom&) = delete;
    KKRandom& operator=(const KKRandom&) = delete;
    KKRandom(KKRandom&&) = delete;
    KKRandom& operator=(KKRandom&&) = delete;
};
