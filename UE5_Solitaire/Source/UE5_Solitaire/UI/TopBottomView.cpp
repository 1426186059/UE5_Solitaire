#include "TopBottomView.h"
#include "MainUIWidget.h"
#include "GameBeginWidget.h"

void TopBottomView::Init(UMainUIWidget* root_uw)
{
    this->mUIRoot = root_uw;
    this->mUIRoot->SetVisibility(ESlateVisibility::Visible);

    this->textFenShu = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textFenShu"));
    this->textTime = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textTime"));
    this->textMoves = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textMoves"));
    this->textRemainCount = Cast<UTextBlock>(this->mUIRoot->GetWidgetFromName("textRemainCount"));

    this->undoNodeBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("undoNode"));
    this->undoNodeBtn->OnClicked.AddDynamic(this->mUIRoot, &UMainUIWidget::OnBtnClicked_UndoBtn);

    this->forwardNodeBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("forwardNode"));
    this->forwardNodeBtn->OnClicked.AddDynamic(this->mUIRoot, &UMainUIWidget::OnBtnClicked_ForwardBtn);

    this->gameNode = Cast<UButton>(this->mUIRoot->GetWidgetFromName("gameNode"));
    this->gameNode->OnClicked.AddDynamic(this->mUIRoot, &UMainUIWidget::OnBtnClicked_GameNodeBtn);

    this->themeNode = Cast<UButton>(this->mUIRoot->GetWidgetFromName("themeNode"));
    this->SettingBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("SettingBtn"));
    //this->hintNodeBtn = Cast<UButton>(this->mUIRoot->GetWidgetFromName("hintNode"));

    this->ChallengeHead = this->mUIRoot->GetWidgetFromName("ChallengeHead");
    this->NormalHead = this->mUIRoot->GetWidgetFromName("NormalHead");
    this->RankHead = this->mUIRoot->GetWidgetFromName("RankHead");

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->AddLambda([this](void* param)
        {
            this->RefreshTopBottomUI();
        });
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateGameModeState)->AddLambda([this](void* param)
        {
            this->UpdateGameModeState();
        });
}

void TopBottomView::OnBtnClicked_GameNodeBtn()
{
    UE_LOG(LogTemp, Log, TEXT("TopBottomView OnBtnClicked_GameNodeBtn"));
    AudioHandler::GetSingleton()->PlaySound("button");
    AKKUIMgr::GetSingleton()->Get<UGameBeginWidget>()->Show();
}

void TopBottomView::OnBtnClicked_UndoBtn()
{
    UE_LOG(LogTemp, Log, TEXT("TopBottomView OnBtnClicked_UndoBtn"));
    //AudioHandler::GetSingleton()->PlaySound("button");
    mUIRoot->PlayRecordUndoAni();
}

void TopBottomView::OnBtnClicked_ForwardBtn()
{
    UE_LOG(LogTemp, Log, TEXT("TopBottomView OnBtnClicked_ForwardBtn"));
    //AudioHandler::GetSingleton()->PlaySound("button");
    mUIRoot->PlayRecordForwardAni();
}

void TopBottomView::RefreshTopBottomUI()
{
    this->textRemainCount->SetText(FText::AsNumber(mUIRoot->mSendCardListGo.Num()));
    this->textFenShu->SetText(FText::AsNumber(RecordStepDataHandler::GetSingleton()->GetData()->nScore));
    this->textMoves->SetText(FText::AsNumber(RecordStepDataHandler::GetSingleton()->GetData()->nMoveCount));
    auto timeSpan = KKTimeTool::GetFormatTimeStr(RecordStepDataHandler::GetSingleton()->GetData()->nTime);
    this->textTime->SetText(FText::FromString(timeSpan));

    //this->UpdateMagicWandState();
    //this->UpdateIQState();
    //this->UpdateCollectALobbyUI();
}

void TopBottomView::UpdateShowHideAni(bool bAuto, bool bShow)
{
    if (bAuto)
    {
        this->bShowBottomUI = !this->bShowBottomUI;
    }
    else
    {
        this->bShowBottomUI = bShow;
    }

    if (this->DownObj == nullptr)
    {
        this->DownObj = this->mUIRoot->GetWidgetFromName("DownObj");
        this->RightObj = this->mUIRoot->GetWidgetFromName("RightObj");
    }

    KKTween::Cancel(this->DownObj);
    KKTween::Cancel(this->RightObj);

    if (this->bShowBottomUI)
    {
        KKTweenExtentions::UMG_MoveLocal_SlotPosY(this->DownObj, 0, 0.3, KKTween::EaseType::easeOutQuad);
        KKTweenExtentions::UMG_MoveLocal_SlotPosX(this->RightObj, 0, 0.3, KKTween::EaseType::easeOutQuad);
        KKTweenExtentions::UMG_Opacity(this->DownObj, 1, 0.3, KKTween::EaseType::easeOutQuad);
        KKTweenExtentions::UMG_Opacity(this->RightObj, 1, 0.3, KKTween::EaseType::easeOutQuad);
    }
    else
    {
        KKTweenExtentions::UMG_MoveLocal_SlotPosY(this->DownObj, 300, 0.3, KKTween::EaseType::easeOutQuad);
        KKTweenExtentions::UMG_MoveLocal_SlotPosX(this->RightObj, 300, 0.3, KKTween::EaseType::easeOutQuad);
        KKTweenExtentions::UMG_Opacity(this->DownObj, 0, 0.3, KKTween::EaseType::easeOutQuad);
        KKTweenExtentions::UMG_Opacity(this->RightObj, 0, 0.3, KKTween::EaseType::easeOutQuad);
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

