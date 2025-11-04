// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWinAniMgr.h"
#include "MainUIWidget.h"

void UGameWinAniMgr::Init(UCanvasPanel* t)
{
    this->go = t;
    this->go->SetVisibility(ESlateVisibility::Visible);
    
    TSubclassOf<UUserWidget> PokerAnimationItemCWBP = LoadClass<UPokerAnimationItemW>(this,
            TEXT("/Game/ResourceABs/MainScene/BPS/UI/PokerAnimationItemCWBP.PokerAnimationItemCWBP_C"));
    this->mCardItemPool = new KKWidgetPool<UPokerAnimationItemW>();
    this->mCardItemPool->Init(PokerAnimationItemCWBP, this->go);
    this->mCardItemPool->preLoadObj(200, 1000);
}

void UGameWinAniMgr::Show()
{
    this->go->SetVisibility(ESlateVisibility::Visible);
}

void UGameWinAniMgr::Hide()
{
    this->go->SetVisibility(ESlateVisibility::Hidden);
}

void UGameWinAniMgr::PlayAni(TFunction<void()> finishFunc)
{
    this->DestroyAniNode();
    this->Show();

    // -- 1：自己写的垃圾动画  2 : 上下跳动的动画 3 : Fly To Pool 4 : 心形动画 5 : 铺满屏幕 6 : 菱形动画 7 : 5星动画
    int nWinType = 2;
    //if (DataCenter::GetSingleton()->data->nTotalWinGameCount <= 8)
    //{
    //    nWinType = DataCenter::GetSingleton()->data->nTotalWinGameCount + 1;
    //}
    //else
    //{
    //    nWinType = FMath::random(1, 9);
    //}

    //--nWinType = 9
    if (nWinType == 1)
    {
        /*self.mAni = require "Lua.MainLogic.view.GameWinAni_1_Curve"
            self.mAni:PlayAni(finishFunc)
            elseif nWinType == 2 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_2_TiaoDong"
            self.mAni : PlayAni(finishFunc)
            elseif nWinType == 3 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_3_FlyToPool"
            self.mAni : PlayAni(finishFunc)
            elseif nWinType == 4 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_4_Heart"
            self.mAni : PlayAni(finishFunc)
            elseif nWinType == 5 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_5_FlyFullScreen"
            self.mAni : PlayAni(finishFunc)
            elseif nWinType == 6 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_6_LingXing"
            self.mAni : PlayAni(finishFunc)
            elseif nWinType == 7 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_7_5Star"
            self.mAni : PlayAni(finishFunc)
            elseif nWinType == 8 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_8_butterfly"
            self.mAni : PlayAni(finishFunc)
            elseif nWinType == 9 then
            self.mAni = require "Lua.MainLogic.view.GameWinAni_9_smileface"
            self.mAni : PlayAni(finishFunc)*/
    }
    else
    {
        //ensume(false, nWinType);
    }
}

void UGameWinAniMgr::DestroyAniNode()
{
    this->Hide();
    //if (this->mAni)
    //{
    //    this->mAni->DoDestroyAction();
    //    this->mAni = nullptr;
    //}
}