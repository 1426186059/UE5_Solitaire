#pragma once

#include "GameData.h"
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveGame.generated.h"

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

    UPROPERTY() FRecordStepData tableOpRecord = {};
    UPROPERTY() FStatisticData StatisticData = {};
    UPROPERTY() FThemeData ThemeData = {};

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
    UPROPERTY() FRecordStepData tableOpRecord;
};

UCLASS()
class UE5_SOLITAIRE_API UAllRecordData : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY() TArray<FAllRecordData_fileNameItem> tableFileNameItem;
};