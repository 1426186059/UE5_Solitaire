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

    int nWinType = 2;
    if (DataCenter::GetSingleton()->data->nTotalWinGameCount <= 8)
    {
        nWinType = DataCenter::GetSingleton()->data->nTotalWinGameCount + 1;
    }
    else
    {
        nWinType = KKRandomTool::RandomInt(1, 9);
    }

    //--nWinType = 9
    if (nWinType == 1)
    {
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation2");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView2_Default>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
    }
    else if (nWinType == 2)
    {
       // self.mAni = require "Lua.MainLogic.view.GameWinAni_2_TiaoDong"
         //   self.mAni : PlayAni(finishFunc)
    }
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
            self.mAni : PlayAni(finishFunc)
    }
    else
    {
        ensureMsgf(false, TEXT("nWinType: %d"), nWinType);
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