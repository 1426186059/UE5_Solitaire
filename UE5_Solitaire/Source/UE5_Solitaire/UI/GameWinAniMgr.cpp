#include "GameWinAniMgr.h"
#include "MainUIWidget.h"

void UGameWinAniMgr::Init(UCanvasPanel* t)
{
    this->go = t;
    ensureMsgf(go, TEXT("UGameWinAniMgr this->go == nil"));

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
    this->mFinishFunc = finishFunc;
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
        TSubclassOf<UUserWidget> PokerAnimationItemCWBP = LoadClass<UPokerAnimationItemW>(this,
            TEXT("/Game/ResourceABs/MainScene/BPS/UI/PokerAnimationItemCWBP.PokerAnimationItemCWBP_C"));

        //var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation2");
        //auto node = Instantiate(prefab) as GameObject;
        //node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        //var mAnimationView = node.GetComponent<AnimationView2_Default>();
        //mAnimationView.gameObject.SetActive(true);
        //mAnimationView.Show(colors, startPt_w, offsetX, callback);
        //this.animationNode = mAnimationView.gameObject;
    }
    else if (nWinType == 2)
    {
       // self.mAni = require "Lua.MainLogic.view.GameWinAni_2_TiaoDong"
         //   self.mAni : PlayAni(finishFunc)
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

TArray<int32> UGameWinAniMgr::GetTableA4Color()
{
    return AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GetTableA4Color();
}

TArray<FVector2D> UGameWinAniMgr::GetTableA4WorldPos()
{
    return AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GetTableA4WorldPos();
}