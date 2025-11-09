#include "TopBottomView.h"
#include "MainUIWidget.h"

void TopBottomView::Init(UMainUIWidget* root_uw)
{
    this->mRootUW = root_uw;
    this->mUIRoot = Cast<UCanvasPanel>(this->mRootUW->GetWidgetFromName("goTopBottomView"));
    this->mUIRoot->SetVisibility(ESlateVisibility::Visible);

    this->textFenShu = Cast<UTextBlock>(this->mRootUW->GetWidgetFromName("textFenShu"));
    this->textTime = Cast<UTextBlock>(this->mRootUW->GetWidgetFromName("textTime"));
    this->textMoves = Cast<UTextBlock>(this->mRootUW->GetWidgetFromName("textMoves"));
    this->textRemainCount = Cast<UTextBlock>(this->mRootUW->GetWidgetFromName("textRemainCount"));

    this->hintNodeBtn = Cast<UButton>(this->mRootUW->GetWidgetFromName("hintNode"));
    //this->hintNodeBtn.onClick.AddListener(function()
    //    AudioHandler:PlaySound("button")

    //    ThemeSolitaire.GameView.fIQTime = 0
    //    ThemeSolitaire.DataCenter : AddIQValue(-2)
    //    ThemeSolitaire.GameView : PlayHintAni()
    //    end)

    this->undoNodeBtn = Cast<UButton>(this->mRootUW->GetWidgetFromName("undoNode"));
    //this->undoNodeBtn.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameView : PlayRecordUndoAni()
    //    end)

    this->forwardNodeBtn = Cast<UButton>(this->mRootUW->GetWidgetFromName("forwardNode"));
    //this->forwardNodeBtn.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameView : PlayRecordForwardAni()
    //    end)

    this->gameNode = Cast<UButton>(this->mRootUW->GetWidgetFromName("gameNode"));
    //this->gameNode.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameBeginView : Show()
    //    end)

    this->themeNode = Cast<UButton>(this->mRootUW->GetWidgetFromName("themeNode"));
    //this->themeNode.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameThemeView : Show()
    //    end)

    this->SettingBtn = Cast<UButton>(this->mRootUW->GetWidgetFromName("SettingBtn"));
    //this->SettingBtn.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameSetView : Show()
    //    end)

    this->textIQMode = Cast<UTextBlock>(this->mRootUW->GetWidgetFromName("IQMode"));
    this->goMagicwandTipCount = this->mRootUW->GetWidgetFromName("bt_magicwand_goTip");
    this->textMagicwandCount = Cast<UTextBlock>(this->mRootUW->GetWidgetFromName("bt_magicwand_goTip_lb_newcount"));
    this->bt_magicwand = Cast<UButton>(this->mRootUW->GetWidgetFromName("bt_magicwand"));

    /* this->bt_magicwand.onClick : AddListener(function()
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
             end)*/

             //this->ChallengeHead = this->mUIRoot:FindDeepChild("ChallengeHead").gameObject
             //this->NormalHead = this->mUIRoot : FindDeepChild("NormalHead").gameObject
             //this->TripHead = this->mUIRoot : FindDeepChild("MissionHead").gameObject
             //this->TripTaskInfo = this->mUIRoot : FindDeepChild("TripTaskInfo").gameObject
             //this->GameTitle2 = this->mUIRoot : FindDeepChild("GameTitle2").gameObject
             //this->RankHead = this->mUIRoot : FindDeepChild("RankHead").gameObject

    //this->imageProgress = this->mUIRoot : FindDeepChild("CollectNode/n_mask/n_Progress") : GetComponent(typeof(UnityUI.Image))
    //    this->AIcon = this->mUIRoot : FindDeepChild("CollectNode/AIcon") : GetComponent(typeof(UnityUI.Image))
    //    this->goCollectFinishEffect = this->mUIRoot : FindDeepChild("CollectNode/goFinishEffect").gameObject
    //    this->goCollectEffect = this->mUIRoot : FindDeepChild("CollectNode/goCollectEffect").gameObject
    //    local ItemPoolGenerator = require "Lua/SimpleFramework/ObjectPool/ItemPool"
    //    local SpineEffect = require "Lua.Effect.SpineEffect"
    //    this->spineCollectEffectPool = ItemPoolGenerator : New(this->goCollectEffect, SpineEffect, 0)

    //    this->collectABtn = this->mUIRoot : FindDeepChild("CollectNode") : GetComponent(typeof(UnityUI.Button))
    //    this->collectABtn.onClick : AddListener(function()
    //        AudioHandler:PlaySound("button")
    //        ThemeSolitaire.GameCollectAView : Show()
    //        end)

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->AddRaw(this, &TopBottomView::RefreshTopBottomUI);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateGameModeState)->AddRaw(this, &TopBottomView::UpdateGameModeState);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateMagicWandState)->AddRaw(this, &TopBottomView::UpdateMagicWandState);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateTripState)->AddRaw(this, &TopBottomView::UpdateTripState);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateIQState)->AddRaw(this, &TopBottomView::UpdateIQState);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateCollectState)->AddRaw(this, &TopBottomView::UpdateCollectState);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_PlayCollectAAni)->AddRaw(this, &TopBottomView::PlayCollectAAni);
}

void TopBottomView::RefreshTopBottomUI()
{
    auto GameView = AKKUIMgr::GetSingleton()->Get<UMainUIWidget>();
    this->textRemainCount.text = GameView->mSendCardListGo.Num();
    this->textFenShu.text = RecordStepDataHandler::GetSingleton()->GetData()->nScore;
    this->textMoves.text = RecordStepDataHandler::GetSingleton()->GetData()->nMoveCount;
    auto timeSpan = CS.System.TimeSpan.FromSeconds(RecordStepDataHandler::GetSingleton()->GetData()->nTime);
    this->textTime.text = timeSpan:ToString("mm\\:ss");
    this->UpdateMagicWandState();
    this->UpdateIQState();
    this->UpdateCollectALobbyUI();
}

void TopBottomView::UpdateShowHideAni(bool bShow)
{
    if (bShow)
    {
        this->bShowBottomUI = bShow;
    }
    else
    {
        this->bShowBottomUI = !this->bShowBottomUI;
    }

    if (this->DownObj == nullptr)
    {
        this->DownObj = this->mUIRoot->GetWidgetFromName("Bottom_DownObj");
        this->goDownObjCanvasGroup = this->DownObj->GetComponent(typeof(Unity.CanvasGroup));
        this->RightObj = this->mUIRoot->GetWidgetFromName("Bottom_RightObj");
        this->goRightObjCanvasGroup = this->RightObj->GetComponent(typeof(Unity.CanvasGroup));
    }

    LeanTween.cancel(this->DownObj);
    LeanTween.cancel(this->RightObj);

    if (this->bShowBottomUI)
    {
        LeanTween.moveLocalY(this->DownObj, 0, 0.3) :setEase(LeanTweenType.easeOutQuad);
        LeanTween.moveLocalX(this->RightObj, 0, 0.3) : setEase(LeanTweenType.easeOutQuad);
        LeanTween.value(0, 1, 0.3) : setEase(LeanTweenType.easeOutQuad) : setOnUpdate(function(value)
            this->goDownObjCanvasGroup.alpha = value
            this->goRightObjCanvasGroup.alpha = value
            end)
    }
    else
    {
        LeanTween.moveLocalY(this->DownObj, -300, 0.3) :setEase(LeanTweenType.easeOutQuad);
        LeanTween.moveLocalX(this->RightObj, 300, 0.3) : setEase(LeanTweenType.easeOutQuad);
        LeanTween.value(1, 0, 0.3) : setEase(LeanTweenType.easeOutQuad) : setOnUpdate(function(value)
            this->goDownObjCanvasGroup.alpha = value
            this->goRightObjCanvasGroup.alpha = value
            end)
        end
    }
}

