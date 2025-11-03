// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData.generated.h"

USTRUCT()
struct FRecordStepData_OpStepItem
{
    GENERATED_BODY()

public:
    UPROPERTY() TArray<int32> fromPosTypeInfo = {};
    UPROPERTY() TArray<int32> toPosTypeInfo = {};
    UPROPERTY() TArray<int32> mSelectCardList = {};
    UPROPERTY() int32 nScore = 0;
    UPROPERTY() int32 nTureOverPokerId = 0;
};

USTRUCT()
struct FRecordStepData
{
    GENERATED_BODY()

public:
    UPROPERTY() int32 nGameMode = 0;
    UPROPERTY() int32 nMoveCount = 0;
    UPROPERTY() int32 nTime = 0;
    UPROPERTY() int32 nScore = 0;
    UPROPERTY() bool bWin = false;
    UPROPERTY() TArray<int32> mInitSendCardList = {};
    UPROPERTY() TArray<FRecordStepData_OpStepItem> tableOpStepItem = {};
    UPROPERTY() int32 nUndoCount = 0;
};

USTRUCT()
struct FThemeData
{
    GENERATED_BODY()

public:
    UPROPERTY() int64 nTimeStamp = 0;
    UPROPERTY() TArray<int32> array_themeids = {};
    UPROPERTY() TArray<int32> array_bgids = {};
    UPROPERTY() TArray<int32> array_frontids = {};
    UPROPERTY() TArray<int32> array_backids = {};
    UPROPERTY() int32 themeBgId = 0;
    UPROPERTY() int32 themeBackId = 0;
    UPROPERTY() int32 themeZhengId = 0;
    UPROPERTY() int32 themePaiId = 0;
};

USTRUCT()
struct FAllRecordData_fileNameItem
{
    GENERATED_BODY()
public:
    UPROPERTY() FString fileName;
    UPROPERTY() int64 nTimeStamp;
    UPROPERTY() int32 nGameMode;
    UPROPERTY() bool bFinish;
};

USTRUCT()
struct FStatisticData
{
    GENERATED_BODY()

public:
    UPROPERTY() int32 nGameCount = 0;
    UPROPERTY() int32 nGameWinCount = 0;
    UPROPERTY() int32 nSumScore = 0;
    UPROPERTY() int32 nAverageScore = 0;
    UPROPERTY() int32 nMaxScore = 0;
    UPROPERTY() int32 nSumMoveCount = 0;
    UPROPERTY() int32 nAverageMoveCount = 0;
    UPROPERTY() int32 nMinMoveCount = 0;
    UPROPERTY() int32 nSumTime = 0;
    UPROPERTY() int32 nAverageTime = 0;
    UPROPERTY() int32 nMinTime = 0;
    UPROPERTY() int32 nNowWinningStreakCount = 0;
    UPROPERTY() int32 nMaxWinningStreakCount = 0;
};
