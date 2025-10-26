// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KKSingleton.h"
#include <random>

class KKRandomTool:public KKSingleton<KKRandomTool>
{
    friend class KKSingleton<KKRandomTool>;
public:
    int RandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        int random_number = dist(mRandom);
        return random_number;
    }

private:
    std::mt19937 mRandom;

    KKRandomTool()
    {
        std::random_device rd;        // 非确定性种子
        mRandom = std::mt19937(rd()); // Mersenne Twister 引擎
    }

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
        mRandom = std::mt19937(Seed); //Mersenne Twister 引擎
    }

    int RandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        int random_number = dist(mRandom);
        return random_number;
    }

private:
    std::mt19937 mRandom;
    KKRandom(const KKRandom&) = delete;
    KKRandom& operator=(const KKRandom&) = delete;
    KKRandom(KKRandom&&) = delete;
    KKRandom& operator=(KKRandom&&) = delete;
};
