// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_MULTICAST_DELEGATE(ActionDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(Action_Float_Delegate, float);

class UE5_SOLITAIRE_API GameConst
{
private:
    // 1. 私有构造函数：禁止实例化
    GameConst() = delete;
    // 2. 禁止拷贝
    GameConst(const GameConst&) = delete;
    GameConst& operator=(const GameConst&) = delete;
public:
    static const int EventId_InitSceneDoFinishOK = 1;
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
