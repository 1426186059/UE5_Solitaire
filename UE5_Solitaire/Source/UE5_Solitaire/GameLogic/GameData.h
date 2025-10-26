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
    UGameData();

public:
    UPROPERTY() FString langName;
    UPROPERTY() int32  nUIStyle = 0;
    UPROPERTY() int32 nCoinCount = 0;
    UPROPERTY() int32 nNomalModeTotalWinCount = 0;
    UPROPERTY() int32 nTotalGameCount = 0;
    UPROPERTY() int32 nTotalWinGameCount = 0;

    //TArray<int> tableOpRecord = {};
    UPROPERTY() URecordStepDataHandlerData* tableOpRecord = {};
    //data.DailyChallengeData = {}
    //    data.StatisticData = {}
    //    data.ThemeData = {}
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
class UE5_SOLITAIRE_API URecordStepDataHandlerData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY() int32 nGameMode = 0;
    UPROPERTY() int32 nMoveCount = 0;
    UPROPERTY() int32 nTime = 0;
    UPROPERTY() int32 nScore = 0;
    UPROPERTY() bool bWin = false;
    UPROPERTY() TArray<int32> mInitSendCardList = {};
    UPROPERTY() TArray<URecordStepDataHandlerData_OpStepItem*> tableOpStepItem = {};
    UPROPERTY() int32 nUndoCount = 0;
};

UCLASS()
class UE5_SOLITAIRE_API URecordStepDataHandlerData_OpStepItem : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY() TArray<int32> fromPosTypeInfo = {};
    UPROPERTY() TArray<int32> toPosTypeInfo = {};
    UPROPERTY() TArray<int32> mSelectCardList = {};
    UPROPERTY() int32 nScore = 0;
    UPROPERTY() int32 nTureOverPokerId = 0;
};
