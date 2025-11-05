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

    nWinType = 1;
    if (nWinType == 1)
    {
        FString path = TEXT("/Game/ResourceABs/MainScene/BPS/UI/WinAnimation/WinAni_Default_CWBP.WinAni_Default_CWBP_C");
        auto mInstance = AKKUIMgr::GetSingleton()->GetByFullPath<UAnimationView2_Default_Widget>(*path);
        UMGHelper::SetParent(mInstance, this->go);
        UMGHelper::SetSlotAnchor(mInstance, FAnchors(0, 0, 1, 1));
        UMGHelper::SetSlotAlignment(mInstance, FVector2D(0.5));
        UMGHelper::SetSlotPos(mInstance, FVector2D(0));
        UMGHelper::SetSlotSize(mInstance, FVector2D(0));
        mInstance->SetVisibility(ESlateVisibility::Visible);
        mInstance->Init(this);
        mInstance->PlayAni();
        this->mWinAniInstance = mInstance;
    }
    else if (nWinType == 2)
    {
       // self.mAni = require "Lua.MainLogic.view.GameWinAni_2_TiaoDong"
       // self.mAni : PlayAni(finishFunc)
    }
    else
    {
        ensureMsgf(false, TEXT("nWinType: %d"), nWinType);
    }
}

void UGameWinAniMgr::DestroyAniNode()
{
    this->Hide();
    if (this->mWinAniInstance)
    {
        this->mWinAniInstance->DoDestroyAction();
        this->mWinAniInstance = nullptr;
    }
}

TArray<int32> UGameWinAniMgr::GetTableA4Color()
{
    return AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GetTableA4Color();
}

TArray<FVector2D> UGameWinAniMgr::GetTableA4WorldPos()
{
    return AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GetTableA4WorldPos();
}