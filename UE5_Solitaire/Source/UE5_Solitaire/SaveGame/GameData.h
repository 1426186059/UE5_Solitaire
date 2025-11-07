// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
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
    UPROPERTY() int32 nScore = {};
    UPROPERTY() int32 nTureOverPokerId = {};

    bool operator==(const FRecordStepData_OpStepItem& Rhs) const
    {
        return 
            TArrayExtentions::orTArrayEqual(fromPosTypeInfo, Rhs.fromPosTypeInfo) &&
            TArrayExtentions::orTArrayEqual(toPosTypeInfo, Rhs.toPosTypeInfo) &&
            TArrayExtentions::orTArrayEqual(mSelectCardList, Rhs.mSelectCardList) &&
            nScore == Rhs.nScore && 
            nTureOverPokerId == Rhs.nTureOverPokerId;
    }
};

USTRUCT()
struct FRecordStepData
{
    GENERATED_BODY()

public:
    UPROPERTY() int32 nGameMode = {};
    UPROPERTY() int32 nMoveCount = {};
    UPROPERTY() int32 nTime = {};
    UPROPERTY() int32 nScore = {};
    UPROPERTY() bool bWin = {};
    UPROPERTY() TArray<int32> mInitSendCardList = {};
    UPROPERTY() TArray<FRecordStepData_OpStepItem> tableOpStepItem = {};
    UPROPERTY() int32 nUndoCount = {};
};

USTRUCT()
struct FThemeData
{
    GENERATED_BODY()

public:
    UPROPERTY() int64 nTimeStamp = {};
    UPROPERTY() TArray<int32> array_themeids = {};
    UPROPERTY() TArray<int32> array_bgids = {};
    UPROPERTY() TArray<int32> array_frontids = {};
    UPROPERTY() TArray<int32> array_backids = {};
    UPROPERTY() int32 themeBgId = {};
    UPROPERTY() int32 themeBackId = {};
    UPROPERTY() int32 themeZhengId = {};
    UPROPERTY() int32 themePaiId = {};
};

USTRUCT()
struct FAllRecordData_fileNameItem
{
    GENERATED_BODY()
public:
    UPROPERTY() FString fileName = {};
    UPROPERTY() int64 nTimeStamp = {};
    UPROPERTY() int32 nGameMode = {};
    UPROPERTY() bool bFinish = {};
};

USTRUCT()
struct FStatisticData
{
    GENERATED_BODY()

public:
    UPROPERTY() int32 nGameCount = {};
    UPROPERTY() int32 nGameWinCount = {};
    UPROPERTY() int32 nSumScore = {};
    UPROPERTY() int32 nAverageScore = {};
    UPROPERTY() int32 nMaxScore = {};
    UPROPERTY() int32 nSumMoveCount = {};
    UPROPERTY() int32 nAverageMoveCount = {};
    UPROPERTY() int32 nMinMoveCount = {};
    UPROPERTY() int32 nSumTime = {};
    UPROPERTY() int32 nAverageTime = {};
    UPROPERTY() int32 nMinTime = {};
    UPROPERTY() int32 nNowWinningStreakCount = {};
    UPROPERTY() int32 nMaxWinningStreakCount = {};
};
