#include "TopBottomView.h"
#include "MainUIWidget.h"

void TopBottomView::Init(UMainUIWidget* root_uw)
{
    this->mUIRoot = root_uw;
    this->mUIRoot->SetVisibility(ESlateVisibility::Visible);

    this->textFenShu = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textFenShu"));
    this->textTime = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textTime"));
    this->textMoves = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textMoves"));
    this->textRemainCount = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textRemainCount"));

    this->hintNodeBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("hintNode"));
    //this->hintNodeBtn.onClick.AddListener(function()
    //    AudioHandler:PlaySound("button")

    //    ThemeSolitaire.GameView.fIQTime = 0
    //    ThemeSolitaire.DataCenter : AddIQValue(-2)
    //    ThemeSolitaire.GameView : PlayHintAni()
    //    end)

    this->undoNodeBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("undoNode"));
    //this->undoNodeBtn.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameView : PlayRecordUndoAni()
    //    end)

    this->forwardNodeBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("forwardNode"));
    //this->forwardNodeBtn.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameView : PlayRecordForwardAni()
    //    end)

    this->gameNode = Cast<UButton>(this->mUIRoot->GetWidgetFromName("gameNode"));
    //this->gameNode.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameBeginView : Show()
    //    end)

    this->themeNode = Cast<UButton>(this->mUIRoot->GetWidgetFromName("themeNode"));
    //this->themeNode.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameThemeView : Show()
    //    end)

    this->SettingBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("SettingBtn"));
    //this->SettingBtn.onClick : AddListener(function()
    //    AudioHandler:PlaySound("button")
    //    ThemeSolitaire.GameSetView : Show()
    //    end)

    this->textIQMode = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("IQMode"));
    this->goMagicwandTipCount = this->mUIRoot->GetWidgetFromName("bt_magicwand_goTip");
    this->textMagicwandCount = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("bt_magicwand_goTip_lb_newcount"));
    this->bt_magicwand = Cast<UButton>(this->mUIRoot->GetWidgetFromName("bt_magicwand"));

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
    //KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateMagicWandState)->AddRaw(this, &TopBottomView::UpdateMagicWandState);
    //KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateTripState)->AddRaw(this, &TopBottomView::UpdateTripState);
    //KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateIQState)->AddRaw(this, &TopBottomView::UpdateIQState);
    //KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateCollectState)->AddRaw(this, &TopBottomView::UpdateCollectState);
    //KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_PlayCollectAAni)->AddRaw(this, &TopBottomView::PlayCollectAAni);
}

void TopBottomView::RefreshTopBottomUI()
{
    auto GameView = AKKUIMgr::GetSingleton()->Get<UMainUIWidget>();
    this->textRemainCount->SetText(FText::AsNumber(GameView->mSendCardListGo.Num()));
    this->textFenShu->SetText(FText::AsNumber(RecordStepDataHandler::GetSingleton()->GetData()->nScore));
    this->textMoves->SetText(FText::AsNumber(RecordStepDataHandler::GetSingleton()->GetData()->nMoveCount));
    auto timeSpan = KKTimeTool::GetFormatTimeStr(RecordStepDataHandler::GetSingleton()->GetData()->nTime);
    this->textTime->SetText(FText::FromString(timeSpan));

    //this->UpdateMagicWandState();
    //this->UpdateIQState();
    //this->UpdateCollectALobbyUI();
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
        this->RightObj = this->mUIRoot->GetWidgetFromName("Bottom_RightObj");
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

void TopBottomView::UpdateGameModeState()
{
    this->ChallengeHead->SetVisibility(ESlateVisibility::Hidden);
    this->NormalHead->SetVisibility(ESlateVisibility::Hidden);
    this->RankHead->SetVisibility(ESlateVisibility::Hidden);

    if (this->mUIRoot->nGameMode == SolitaireGameMode::Rank)
    {
        this->RankHead->SetVisibility(ESlateVisibility::Visible);
    }
    else if (this->mUIRoot->nGameMode == SolitaireGameMode::DailyChallenge)
    {
        this->ChallengeHead->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        this->NormalHead->SetVisibility(ESlateVisibility::Visible);
    }

}

