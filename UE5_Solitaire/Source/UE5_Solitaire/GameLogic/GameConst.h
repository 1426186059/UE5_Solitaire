// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class GameConst
{
public:
    static const int EventId_InitSceneDoFinishOK = 1;
    static const int EventId_UpdatePokerAtlas = 2;
    static const int EventId_RefreshTopBottomUI = 3;
    static const int EventId_UpdateGameModeState = 4;
    static const int EventId_UpdateTripState = 5;

    DECLARE_MULTICAST_DELEGATE(ActionDelegate);
    DECLARE_MULTICAST_DELEGATE_OneParam(Action_Float_Delegate, float);

private:
    // 禁止拷贝和移动（单例不应被复制）
    GameConst() = delete;
    ~GameConst() = delete;
    GameConst(const GameConst&) = delete;
    GameConst& operator=(const GameConst&) = delete;
    GameConst(GameConst&&) = delete;
    GameConst& operator=(GameConst&&) = delete;
};

enum SolitaireGameMode
{
    Normal = 1,
    DailyChallenge = 2,
    Trip = 3,
    Rank = 4,
};

enum SolitaireColorType
{
    Spade = 4, //黑桃
    Heart = 2, //红桃
    Club = 1,  // 梅花
    Diamond = 3, //方片
};

enum SolitairePokerPosType : int32
{
    Top7Pos = 1,
    A4Pos = 2,
    Draw3Pos = 3,
    SendPokerPos = 4,
};
