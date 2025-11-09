#include "GameWinAniMgr.h"
#include "MainUIWidget.h"

void TopBottomView::Init(UCanvasPanel* goTopBottomView)
{
    this->mUIRoot = goTopBottomView;
    this->mUIRoot.gameObject:SetActive(true);

    this->textFenShu = this->mUIRoot->FindDeepChild("textFenShu") : GetComponentInChildren(typeof(UnityUI.Text));
    this->textTime = this->mUIRoot : FindDeepChild("textTime") : GetComponentInChildren(typeof(UnityUI.Text));
    this->textMoves = this->mUIRoot : FindDeepChild("textMoves") : GetComponentInChildren(typeof(UnityUI.Text));
    this->textRemainCount = ThemeSolitaire.GameView.mUIRoot : FindDeepChild("textRemainCount") : GetComponentInChildren(typeof(UnityUI.Text));

        this->hintNodeBtn = this->mUIRoot : FindDeepChild("hintNode") : GetComponentInChildren(typeof(UnityUI.Button))
        this->hintNodeBtn.onClick : AddListener(function()
            AudioHandler:PlaySound("button")

            ThemeSolitaire.GameView.fIQTime = 0
            ThemeSolitaire.DataCenter : AddIQValue(-2)
            ThemeSolitaire.GameView : PlayHintAni()
            end)

        this->undoNodeBtn = this->mUIRoot:FindDeepChild("undoNode") : GetComponentInChildren(typeof(UnityUI.Button))
        this->undoNodeBtn.onClick : AddListener(function()
            AudioHandler:PlaySound("button")
            ThemeSolitaire.GameView : PlayRecordUndoAni()
            end)

        this->forwardNodeBtn = this->mUIRoot : FindDeepChild("forwardNode") : GetComponentInChildren(typeof(UnityUI.Button))
        this->forwardNodeBtn.onClick : AddListener(function()
            AudioHandler:PlaySound("button")
            ThemeSolitaire.GameView : PlayRecordForwardAni()
            end)

        this->gameNode = this->mUIRoot : FindDeepChild("gameNode") : GetComponentInChildren(typeof(UnityUI.Button))
        this->gameNode.onClick : AddListener(function()
            AudioHandler:PlaySound("button")
            ThemeSolitaire.GameBeginView : Show()
            end)

        this->themeNode = this->mUIRoot : FindDeepChild("themeNode") : GetComponentInChildren(typeof(UnityUI.Button))
        this->themeNode.onClick : AddListener(function()
            AudioHandler:PlaySound("button")
            ThemeSolitaire.GameThemeView : Show()
            end)

        this->SettingBtn = this->mUIRoot : FindDeepChild("SettingBtn") : GetComponentInChildren(typeof(UnityUI.Button))
        this->SettingBtn.onClick : AddListener(function()
            AudioHandler:PlaySound("button")
            ThemeSolitaire.GameSetView : Show()
            end)

        this->textIQMode = this->mUIRoot : FindDeepChild("IQMode") : GetComponent(typeof(UnityUI.Text))
        this->goMagicwandTipCount = this->mUIRoot : FindDeepChild("bt_magicwand/goTip").gameObject
        this->textMagicwandCount = this->mUIRoot : FindDeepChild("bt_magicwand/goTip/lb_newcount") : GetComponent(typeof(UnityUI.Text))
        this->bt_magicwand = this->mUIRoot : FindDeepChild("bt_magicwand") : GetComponentInChildren(typeof(UnityUI.Button))
        this->bt_magicwand.onClick : AddListener(function()
            AudioHandler:PlaySound("mofabang")

            if ThemeSolitaire.DataCenter.data.nMagicWandCount > 0 then
                if ThemeSolitaire.GameView:orCanUseMagicWand() then
                    ThemeSolitaire.GameView : UseMagicWand()
                    GameEventHandler : Brocast(ThemeSolitaire.EventName.UpdateMagicWandState)
                else
                    CommonTipPoolView:Show("No useable moves")
                    end
            else
                ThemeSolitaire.GameMagicWandView:Show()
                end
                end)

        this->ChallengeHead = this->mUIRoot:FindDeepChild("ChallengeHead").gameObject
        this->NormalHead = this->mUIRoot : FindDeepChild("NormalHead").gameObject
        this->TripHead = this->mUIRoot : FindDeepChild("MissionHead").gameObject
        this->TripTaskInfo = this->mUIRoot : FindDeepChild("TripTaskInfo").gameObject
        this->GameTitle2 = this->mUIRoot : FindDeepChild("GameTitle2").gameObject
        this->RankHead = this->mUIRoot : FindDeepChild("RankHead").gameObject

        this->imageProgress = this->mUIRoot : FindDeepChild("CollectNode/n_mask/n_Progress") : GetComponent(typeof(UnityUI.Image))
        this->AIcon = this->mUIRoot : FindDeepChild("CollectNode/AIcon") : GetComponent(typeof(UnityUI.Image))
        this->goCollectFinishEffect = this->mUIRoot : FindDeepChild("CollectNode/goFinishEffect").gameObject
        this->goCollectEffect = this->mUIRoot : FindDeepChild("CollectNode/goCollectEffect").gameObject
        local ItemPoolGenerator = require "Lua/SimpleFramework/ObjectPool/ItemPool"
        local SpineEffect = require "Lua.Effect.SpineEffect"
        this->spineCollectEffectPool = ItemPoolGenerator : New(this->goCollectEffect, SpineEffect, 0)

        this->collectABtn = this->mUIRoot : FindDeepChild("CollectNode") : GetComponent(typeof(UnityUI.Button))
        this->collectABtn.onClick : AddListener(function()
            AudioHandler:PlaySound("button")
            ThemeSolitaire.GameCollectAView : Show()
            end)

        GameEventHandler : AddListener(ThemeSolitaire.EventName.RefreshTopBottomUI, self)
        GameEventHandler : AddListener(ThemeSolitaire.EventName.UpdateGameModeState, self)
        GameEventHandler : AddListener(ThemeSolitaire.EventName.UpdateMagicWandState, self)
        GameEventHandler : AddListener(ThemeSolitaire.EventName.UpdateTripState, self)
        GameEventHandler : AddListener(ThemeSolitaire.EventName.UpdateIQState, self)

        GameEventHandler : AddListener(ThemeSolitaire.EventName.UpdateCollectState, self)
        GameEventHandler : AddListener(ThemeSolitaire.EventName.PlayCollectAAni, self)
}

void TopBottomView::Show()
{
    this->go->SetVisibility(ESlateVisibility::Visible);
}

void TopBottomView::Hide()
{
    this->go->SetVisibility(ESlateVisibility::Hidden);
}

void TopBottomView::PlayAni(TFunction<void()> finishFunc)
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
        auto mInstance = AKKUIMgr::GetSingleton()->Load<UAnimationView2_Default_Widget>(*path);
        UMGHelper::SetParent(mInstance, this->go);
        UMGHelper::SetSlotAnchor(mInstance, FAnchors(0, 0, 1, 1));
        UMGHelper::SetSlotAlignment(mInstance, FVector2D(0.5));
        UMGHelper::SetSlotPos(mInstance, FVector2D(0));
        UMGHelper::SetSlotSize(mInstance, FVector2D(0));
        mInstance->SetMgr(this);
        mInstance->Show();
        this->mWinAniInstance = mInstance;
    }
    else if (nWinType == 2)
    {
       // this->mAni = require "Lua.MainLogic.view.GameWinAni_2_TiaoDong"
       // this->mAni : PlayAni(finishFunc)
    }
    else
    {
        ensureMsgf(false, TEXT("nWinType: %d"), nWinType);
    }
}

void TopBottomView::DestroyAniNode()
{
    this->Hide();
    if (this->mWinAniInstance)
    {
        this->mWinAniInstance->DoDestroyAction();
        this->mWinAniInstance = nullptr;
    }
}

TArray<int32> TopBottomView::GetTableA4Color()
{
    return AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GetTableA4Color();
}

TArray<FVector2D> TopBottomView::GetTableA4WorldPos()
{
    return AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GetTableA4WorldPos();
}