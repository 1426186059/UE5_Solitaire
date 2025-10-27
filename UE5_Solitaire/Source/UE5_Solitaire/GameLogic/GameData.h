// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameData.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UGameData : public USaveGame
{
	GENERATED_BODY()
public:
    UPROPERTY() FString langName;
    UPROPERTY() int32  nUIStyle = 0;
    UPROPERTY() int32 nCoinCount = 0;
    UPROPERTY() int32 nNomalModeTotalWinCount = 0;
    UPROPERTY() int32 nTotalGameCount = 0;
    UPROPERTY() int32 nTotalWinGameCount = 0;

    UPROPERTY() URecordStepData* tableOpRecord = {};
    UPROPERTY() UStatisticData* StatisticData = {};
    UPROPERTY() UThemeData* ThemeData = {};

    //data.DailyChallengeData = {}
    //    data.DailyTaskData = {}
    //    data.CollectPokerTaskData = {}
    //    data.StampData = {}
    //    data.StageRewardData = {}
    //    data.TripData = {}
    //    data.DailyRankData = {}

    UPROPERTY() int32 nMagicWandCount = 0; //魔杖数量
    UPROPERTY() bool bOpenSound = true;
    UPROPERTY() bool bAutoComplete = true;
    UPROPERTY() bool bClickToMove = true;
    UPROPERTY() bool bAutoHint = false;
    UPROPERTY() bool bFastGame = false;
    UPROPERTY() int32 nDrawCount = 1;
    UPROPERTY() bool bIQMode = false;
    UPROPERTY() bool bLeftHandMode = false;
    UPROPERTY() int32 nMusicIndex = 0;
    UPROPERTY() int32 nDifficultLayer = 1; //难度等级 1～10
    UPROPERTY() int32 nDifficultLayer_ContinueWinCount = 0;
    UPROPERTY() int32 nDifficultLayer_ContinueLoseCount = 0;
    UPROPERTY() int32 nGameLevel = 0; //等级
    UPROPERTY() int32 mStageReward_WinCount = 0;
    UPROPERTY() int32 nIQValue = 100;
};

UCLASS()
class UE5_SOLITAIRE_API UAllRecordData_OpRecord : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY() URecordStepData* tableOpRecord = nullptr;
};

UCLASS()
class UE5_SOLITAIRE_API UAllRecordData : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY() TArray<UAllRecordData_fileNameItem*> tableFileNameItem;
};


UCLASS()
class UE5_SOLITAIRE_API URecordStepData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY() int32 nGameMode = 0;
    UPROPERTY() int32 nMoveCount = 0;
    UPROPERTY() int32 nTime = 0;
    UPROPERTY() int32 nScore = 0;
    UPROPERTY() bool bWin = false;
    UPROPERTY() TArray<int32> mInitSendCardList = {};
    UPROPERTY() TArray<URecordStepData_OpStepItem*> tableOpStepItem = {};
    UPROPERTY() int32 nUndoCount = 0;
};

UCLASS()
class UE5_SOLITAIRE_API URecordStepData_OpStepItem : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY() TArray<int32> fromPosTypeInfo = {};
    UPROPERTY() TArray<int32> toPosTypeInfo = {};
    UPROPERTY() TArray<int32> mSelectCardList = {};
    UPROPERTY() int32 nScore = 0;
    UPROPERTY() int32 nTureOverPokerId = 0;
};

UCLASS()
class UE5_SOLITAIRE_API UThemeData : public UObject
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

UCLASS()
class UE5_SOLITAIRE_API UAllRecordData_fileNameItem : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY() FString fileName;
    UPROPERTY() int64 nTimeStamp;
    UPROPERTY() int32 nGameMode;
    UPROPERTY() bool bFinish;
};

UCLASS()
class UE5_SOLITAIRE_API UStatisticData : public UObject
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
