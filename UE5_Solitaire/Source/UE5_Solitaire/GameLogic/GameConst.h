// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UE5_SOLITAIRE_API GameConst
{
private:
    // 1. Ë½ÓÐ¹¹Ôìº¯Êý£º½ûÖ¹ÊµÀý»¯
    GameConst() = delete;
    // 2. ½ûÖ¹¿½±´
    GameConst(const GameConst&) = delete;
    GameConst& operator=(const GameConst&) = delete;
public:
    static const int EventId_InitSceneDoFinishOK = 1;
    static const int EventId_UpdatePokerAtlas = 2;
    static const int EventId_RefreshTopBottomUI = 3;

    DECLARE_MULTICAST_DELEGATE(ActionDelegate);
    DECLARE_MULTICAST_DELEGATE_OneParam(Action_Float_Delegate, float);
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
    Spade = 4, //ºÚÌÒ
    Heart = 2, //ºìÌÒ
    Club = 1,  // Ã·»¨
    Diamond = 3, //·½Æ¬
};

enum SolitairePokerPosType : int32
{
    Top7Pos = 1,
    A4Pos = 2,
    Draw3Pos = 3,
    SendPokerPos = 4,
};
