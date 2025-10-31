// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWinAniMgr.h"

void GameWinAniMgr::Init(UWidget* t)
{
    /*self.transform = go.transform : GetComponent(typeof(Unity.RectTransform))
    self.transform.gameObject:SetActive(false)
    
    local cardItemPrefab = ThemeSolitaire.mCommonResSerialization : FindPrefab("CardAniItem")
    local CardItemGen = require "Lua.MainLogic.view.GameWinAniItem"
    local ItemPoolGen = require "Lua.SimpleFramework.ObjectPool.ItemPool"

    self.mCardItemPool = ItemPoolGen : New(cardItemPrefab, CardItemGen, 0)
    self.mCardItemPool : SetItemParent(self.transform)
    self.mCardItemPool : preLoadByFrame(60, 500)*/
}

void GameWinAniMgr::Show()
{
    //self.transform.gameObject : SetActive(true)
}

void GameWinAniMgr::Hide()
{
    // self.transform.gameObject : SetActive(false)
}

void GameWinAniMgr::PlayAni(TFunction<void()> finishFunc)
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

void GameWinAniMgr::DestroyAniNode()
{
    this->Hide();
    //if (this->mAni)
    //{
    //    this->mAni->DoDestroyAction();
    //    this->mAni = nullptr;
    //}
}