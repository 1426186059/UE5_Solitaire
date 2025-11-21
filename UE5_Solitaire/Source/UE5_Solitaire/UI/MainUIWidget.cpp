// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"
#include "TopBottomView.h"
#include "Item/PokerItemWidget.h"
#include "Item/PokerAnimationItemW.h"
#include "GMWidget.h"
#include "GameWinWidget.h"

void UMainUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    //if (this->orInit())
    //{
    //    auto mSize = UMGHelper::GetRelativePos(this->PokerItemParent, mUIRoot->GetWidgetFromName("FaPaiPos"));
    //    if (mSize != this->tranFaPaiPos)
    //    {
    //        this->tranFaPaiPos = mSize;
    //        UE_LOG(LogTemp, Log, TEXT("UMainUIWidget tranFaPaiPos 111: %s"), *mSize.ToString());
    //    }
    //}
}

FReply UMainUIWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        UE_LOG(LogTemp, Log, TEXT("Left Mouse Click"));
    }
    else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        UE_LOG(LogTemp, Log, TEXT("Right Mouse Click"));
    }

    //return FReply::Handled();
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

//--------------------------------------------------------------------------------------------------------------------
void UMainUIWidget::OnCreate()
{
    Super::OnCreate();
    UMGHelper::SetParent(this, AKKUIMgr::GetSingleton()->GetRootWidget()->Layer_Base);
    UMGHelper::SetSlotAnchor(this, FAnchors(0, 0, 1, 1));
    UMGHelper::SetSlotAlignment(this, FVector2D(0.5));
    UMGHelper::SetSlotPos(this, FVector2D(0));
    UMGHelper::SetSlotSize(this, FVector2D(0));
}

void UMainUIWidget::OnLayoutChanged()
{
    Super::OnLayoutChanged();
    //BG 适配屏幕
    if (this->mBG == nullptr)
    {
        this->mBG = Cast<UImage>(mUIRoot->GetWidgetFromName(TEXT("BG")));
    }
    UMGAdapterTool::FitBG(mUIRoot, this->mBG);
}

void UMainUIWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget Init"));

    mUIRoot = this;
    if (!mUIRoot)
    {
        UE_LOG(LogTemp, Error, TEXT("mUIRoot == null"));
        return;
    }

    this->PokerItemParent = Cast<UCanvasPanel>(mUIRoot->GetWidgetFromName("PokerItemParent"));
    this->GameWinAniParent = Cast<UCanvasPanel>(mUIRoot->GetWidgetFromName("GameWinAniParent"));

    this->tranFaPaiPos = UMGHelper::GetRLocalPos(this->PokerItemParent, mUIRoot->GetWidgetFromName("FaPaiPos"));
    this->mCardNodeDraw3BeginPos = UMGHelper::GetRLocalPos(this->PokerItemParent, mUIRoot->GetWidgetFromName("Draw3Pos1"));

    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget tranFaPaiPos: %s"), *this->tranFaPaiPos.ToString());
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget mCardNodeDraw3BeginPos: %s"), *this->mCardNodeDraw3BeginPos.ToString());
    
    this->tableCardNode4APos = {};
    for (int32 i = 1; i <= 4; i++)
    {
        FString Key = FString::Printf(TEXT("4APos%d"), i);
        FVector2D mPos = UMGHelper::GetRLocalPos(this->PokerItemParent, mUIRoot->GetWidgetFromName(*Key));
        this->tableCardNode4APos.Add(mPos);
    }

    this->tableCardNodeTop7Pos = {};
    for (int32 i = 1; i <= 7; i++)
    {
        FString Key = FString::Printf(TEXT("Top7Pos%d"), i);
        FVector2D mPos = UMGHelper::GetRLocalPos(this->PokerItemParent, mUIRoot->GetWidgetFromName(*Key));
        this->tableCardNodeTop7Pos.Add(mPos);

        UE_LOG(LogTemp, Log, TEXT("UMainUIWidget tableCardNodeTop7Pos: %s"), *mPos.ToString());
    }
    
    UButton* SendPokerBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("SendPokerBtn"));
    SendPokerBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_SendPokerBtn);
    this->mBG = Cast<UImage>(mUIRoot->GetWidgetFromName("BG"));
    UButton* BGBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("BGBtn"));
    BGBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_BGBtn);
    
    this->GameWinAniMgr = NewObject<UGameWinAniMgr>(this);
    this->GameWinAniMgr->Init(this->GameWinAniParent);
    this->mTopBottomView = TSharedPtr<TopBottomView>(new TopBottomView());
    this->mTopBottomView->Init(this);
    this->mTopBottomView->UpdateShowHideAni(false, true);

    GetWorld()->GetTimerManager().SetTimer(
        this->mTimer, 
        this,
        &UMainUIWidget::TimerPerSecondUpdate,
        1.0f, 
        true
    );
    UEHelper::PauseTimer(this, this->mTimer);

#if WITH_EDITOR
    AKKUIMgr::GetSingleton()->Show<UGMWidget>();
#endif
}

void UMainUIWidget::TimerPerSecondUpdate()
{
    if (!this->orHavePopView())
    {
        RecordStepDataHandler::GetSingleton()->AddTime(1);
        KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);

        if (DataCenter::GetSingleton()->data->bAutoHint)
        {
            this->fRobotThinkingTime = this->fRobotThinkingTime + 1;
            if (this->fRobotThinkingTime > 10.0)
            {
                this->fRobotThinkingTime = 0;
                // this->PlayHintAni();
            }
        }

        this->fIQTime = this->fIQTime + 1;
        if (this->fIQTime > 10)
        {
            this->fIQTime = 0;
            DataCenter::GetSingleton()->AddIQValue(-4);
        }
    }
}

void UMainUIWidget::CheckFirstLayoutOkToShow()
{
    if (!this->orFirstLayoutFinish()) return;

    this->Init();
    this->InitGame();
}

//-----------------------------------------Button 点击事件---------------------------------------------------------------------------
void UMainUIWidget::OnBtnClicked_GameNodeBtn()
{
    this->mTopBottomView->OnBtnClicked_GameNodeBtn();
}

void UMainUIWidget::OnBtnClicked_UndoBtn()
{
    this->mTopBottomView->OnBtnClicked_UndoBtn();
}

void UMainUIWidget::OnBtnClicked_ForwardBtn()
{
    this->mTopBottomView->OnBtnClicked_ForwardBtn();
}

void UMainUIWidget::OnBtnClicked_ThemeBtn()
{
    //this->mTopBottomView->OnBtnClicked_ThemeBtn();
}

void UMainUIWidget::OnBtnClicked_SettingBtn()
{
    //this->mTopBottomView->OnBtnClicked_SettingBtn();
}


void UMainUIWidget::OnBtnClicked_SendPokerBtn()
{
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget OnBtnClicked_SendPokerBtn"));

    this->OnClickChuPai();
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
    this->TellRobot_PlayerAlive();
}

void UMainUIWidget::OnBtnClicked_BGBtn()
{
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget OnBtnClicked_BGBtn"));
    this->TellRobot_PlayerAlive();
    //this->CardHintEffectPool->Reset();
    this->mTopBottomView->UpdateShowHideAni();
}

void UMainUIWidget::RecycleAndInitCardGo()
{
    //------------------------------回收资源------------------------------------
    for (int32 i = 0; i < 7; i++)
    {
        for (int32 j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
        {
            this->mSendCardListGo.Add(tableCardNodeTop7Go[i][j]);
        }
    }
    
    for (int32 i = 0; i < 4; i++)
    {
        for (int32 j = 0; j < this->tableCardNode4AGo[i].Num(); j++)
        {
            this->mSendCardListGo.Add(this->tableCardNode4AGo[i][j]);
        }
    }
    
    for (auto v : this->tableCardDraw3Go)
    {
        this->mSendCardListGo.Add(v);
    }
    
    //----------------------------------初始化资源------------------------------------
    this->tableCardNodeTop7Go = {};
    this->tableCardNode4AGo = {};
    this->tableCardNodeTop7Go.SetNumZeroed(7);
    this->tableCardNode4AGo.SetNumZeroed(4);
    this->tableCardDraw3Go = {};
}

void UMainUIWidget::InitGame()
{
    //初始化播放发牌动画
    mSendCardListGo = {};
    auto PokerItemWBP = LoadClass<UPokerItemWidget>(this,
        TEXT("/Game/ResourceABs/MainScene/BPS/UI/PokerItemCWBP.PokerItemCWBP_C"));

    if (PokerItemWBP == NULL)
    {
        UE_LOG(LogTemp, Error, TEXT("UMainUIWidget PokerItemWBP == null"));
        return;
    }

    for (int32 i = 0; i < 52; i++)
    {
        UPokerItemWidget* mPokerItem = CreateWidget<UPokerItemWidget>(this, PokerItemWBP);
        UMGHelper::SetParent(mPokerItem, this->PokerItemParent);
        UMGHelper::SetSlotAnchor(mPokerItem, FAnchors(0.5, 0.5, 0.5, 0.5));
        UMGHelper::SetSlotAlignment(mPokerItem, FVector2D(0.5, 0.5));
        UMGHelper::SetSlotSize(mPokerItem, FVector2D(0, 0));
        UMGHelper::SetSlotPos(mPokerItem, FVector2D(0));
        UMGHelper::SetRenderPos(mPokerItem, FVector2D(0));
        mPokerItem->Show();
        mSendCardListGo.Add(mPokerItem);
    }

    this->tableCardNodeTop7Go.SetNumZeroed(7);
    this->tableCardNode4AGo.SetNumZeroed(4);
   
    this->RecoverGame(true);
}

void UMainUIWidget::RecoverGame(bool bPlayAni)
{
    auto tableOpStepItem = RecordStepDataHandler::GetSingleton()->GetData()->tableOpStepItem;
    auto mInitSendCardList = RecordStepDataHandler::GetSingleton()->GetData()->mInitSendCardList;
    if (RecordStepDataHandler::GetSingleton()->orGameEnd())
    {
        this->NewGameBegin_ForNormal(true);
        return;
    }
    this->mLastSendCardList = mInitSendCardList;

    //--------------------------- 初始化资源------------------------------
    
    this->RecycleAndInitCardGo();
    this->bGameEnd = false;
    this->nGameMode = (SolitaireGameMode)RecordStepDataHandler::GetSingleton()->GetData()->nGameMode;
    this->PokerItemParent->SetVisibility(ESlateVisibility::Visible);

    ensureMsgf(mInitSendCardList.Num() == 52, TEXT("mInitSendCardList Error: %d"), mInitSendCardList.Num());
    for (int32 i = 0; i < mInitSendCardList.Num(); i++)
    {
        this->mSendCardListGo[i]->SetPokerId(mInitSendCardList[i]);
        this->mSendCardListGo[i]->SetTurnOverState(false);
    }

    for (int32 i = 0; i < 7; i++)
    {
        for (int32 j = 0; j <= i; j++)
        {
            int32 nTopIndex = i;
            UPokerItemWidget* mCardItem = this->mSendCardListGo.Pop();
            this->tableCardNodeTop7Go[nTopIndex].Add(mCardItem);
            UMGHelper::SetSlotPos(mCardItem, this->GetCardNodeTop7MaxHeightPos(nTopIndex));
            if (i == j)
            {
                mCardItem->SetTurnOverState(true);
            }
        }
    }

    RecordStepDataHandler::GetSingleton()->PrintAllOp();
    //this->PrintGameStartInfo();
    for (int i = 0; i < tableOpStepItem.Num(); i++)
    {
        const auto nLastOpInfo = tableOpStepItem[i];
        const auto& oriPosTypeInfo = nLastOpInfo.fromPosTypeInfo;
        const auto& targetPosTypeInfo = nLastOpInfo.toPosTypeInfo;

        if (oriPosTypeInfo[0] == SolitairePokerPosType::SendPokerPos)
        {
            int nSendPokerCount = oriPosTypeInfo[1];
            ensureMsgf(nSendPokerCount <= this->mSendCardListGo.Num(), TEXT("%d, %d"), nSendPokerCount, this->mSendCardListGo.Num());

            int nCount = 0;
            while (nCount < nSendPokerCount)
            {
                nCount++;
                auto mCardItem = TArrayExtentions::Remove(this->mSendCardListGo);
                this->tableCardDraw3Go.Insert(mCardItem, 0);
                mCardItem->SetTurnOverState(true, i - 1);
            }
        }
        else if (oriPosTypeInfo[0] == SolitairePokerPosType::Draw3Pos)
        {
            if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
            {
                auto mCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go, 0);
                auto& mListCardNode4AGo = this->tableCardNode4AGo[targetPosTypeInfo[1]];
                mListCardNode4AGo.Add(mCardItem);
            }
            else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
            {
                auto mCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go, 0);
                auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[targetPosTypeInfo[1]];
                mListCardNodeTop7Go.Add(mCardItem);
            }
            else if (targetPosTypeInfo[0] == SolitairePokerPosType::SendPokerPos)
            {
                while (this->tableCardDraw3Go.Num() > 0)
                {
                    auto mTempCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go, 0);
                    this->mSendCardListGo.Add(mTempCardItem);
                    mTempCardItem->SetTurnOverState(false);
                }
            }
            else
            {
                ensure(false);
            }
        }
        else if (oriPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            auto mCardItem = TArrayExtentions::Remove(this->tableCardNode4AGo[oriPosTypeInfo[1]]);
            if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
            {
                auto& tableCardNode4AGo2 = this->tableCardNode4AGo[targetPosTypeInfo[1]];
                tableCardNode4AGo2.Add(mCardItem);
            }
            else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
            {
                auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[targetPosTypeInfo[1]];
                mListCardNodeTop7Go.Add(mCardItem);
            }
            else
            {
                ensure(false);
            }
        }
        else if (oriPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            int oriTop7Index = oriPosTypeInfo[1];
            int oriIndex = oriPosTypeInfo[2];
            auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[oriTop7Index];
            if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
            {
                bool bUseMagicWand = oriIndex < mListCardNodeTop7Go.Num();
                auto mCardItem = TArrayExtentions::Remove(mListCardNodeTop7Go, oriIndex);
                if (bUseMagicWand)
                {
                    mCardItem->SetTurnOverState(true, i - 1);
                }

                auto& mListCardNode4AGo = this->tableCardNode4AGo[targetPosTypeInfo[1]];
                mListCardNode4AGo.Add(mCardItem);
            }
            else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
            {
                auto taleResult = this->RemoveArrayFromTop7Go(oriPosTypeInfo[1], oriPosTypeInfo[2]);
                this->InsertArrayToTop7Go(targetPosTypeInfo[1], taleResult);
            }
            else
            {
                ensure(false);
            }

            if (mListCardNodeTop7Go.Num() > 0)
            {
                mListCardNodeTop7Go[mListCardNodeTop7Go.Num() - 1]->SetTurnOverState(true, i - 1);
            }
        }
        else
        {
            ensureMsgf(false, TEXT("%d"), oriPosTypeInfo[0]);
        }
    }

    //-------------------------------------------------------------------------- -
    for (auto v : this->mSendCardListGo)
    {
        auto mCardItem = v;
        UMGHelper::SetChildLastZOrder(mCardItem);
        UMGHelper::SetSlotPos(mCardItem, GetCardNodeSendPokerPos());
        mCardItem->Refresh();
        mCardItem->SetEventTriggerState(false);
        mCardItem->Show();
    }

    if (bPlayAni)
    {
        for (int32 i = 0; i < 7; i++)
        {
            for (int32 j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNodeTop7Go[i][j];
                UMGHelper::SetChildLastZOrder(mCardItem);
                mCardItem->Show();
                mCardItem->ForceShowBackUI();
                mCardItem->SetEventTriggerState(mCardItem->orTurnOverStateIsTrue());
                FVector2D fromPos = this->GetCardNodeSendPokerPos();
                UMGHelper::SetSlotPos(mCardItem, fromPos);
            }
        }

        for (int32 i = 0; i < 4; i++)
        {
            for (int32 j = 0; j < this->tableCardNode4AGo[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNode4AGo[i][j];
                UMGHelper::SetChildLastZOrder(mCardItem);
                mCardItem->ForceShowBackUI();
                mCardItem->Show();
                mCardItem->SetEventTriggerState(j + 1 == this->tableCardNode4AGo[i].Num());
                FVector2D fromPos = this->GetCardNodeSendPokerPos();
                UMGHelper::SetSlotPos(mCardItem, fromPos);
            }
        }

        for (int32 i = 0; i < this->tableCardDraw3Go.Num(); i++)
        {
            auto mCardItem = this->tableCardDraw3Go[i];
            UMGHelper::SetChildFirstZOrder(mCardItem);
            mCardItem->ForceShowBackUI();
            mCardItem->SetEventTriggerState(i == 0);

            if (i <= 2)
            {
                mCardItem->Show();
            }
            else
            {
                mCardItem->Hide();
            }

            FVector2D fromPos = this->GetCardNodeSendPokerPos();
            UMGHelper::SetSlotPos(mCardItem, fromPos);
        }

        // ----------------------- 动画播放---------------------------
        AudioHandler::GetSingleton()->PlaySound(TEXT("start_new"));
        for (int32 i = 0; i < 7; i++)
        {
            for (int32 j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNodeTop7Go[i][j];
                auto fromPos = this->GetCardNodeSendPokerPos();
                auto toPos = this->GetCardNodeTop7Pos(i, j);

                auto mTween = KKTweenExtentions::UMG_MoveLocal_SlotPos(mCardItem, toPos, 0.3, KKTween::EaseType::easeOutQuad)->SetOnCompleteFunc(
                    [=, this]()
                    {
                        if (mCardItem->orTurnOverStateIsTrue())
                        {
                            mCardItem->PlayTurnOverAni();
                        }
                    })->SetDelay(0.05 * j);
            }
        }

        KKTween::delayedCall(0.5, [=, this]()
            {
                int n4ACount = 0;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < this->tableCardNode4AGo[i].Num(); j++)
                    {
                        n4ACount++;
                        auto mCardItem = this->tableCardNode4AGo[i][j];
                        auto fromPos = this->GetCardNodeSendPokerPos();
                        auto toPos = this->GetCardNode4APos(i);

                        auto mTween = KKTweenExtentions::UMG_MoveLocal_SlotPos(mCardItem, toPos, 0.3, KKTween::EaseType::easeOutQuad)->SetOnCompleteFunc(
                            [=, this]()
                            {
                                if (mCardItem->orTurnOverStateIsTrue())
                                {
                                    mCardItem->PlayTurnOverAni();
                                }
                            });
                    }
                }

                if (n4ACount > 0)
                {
                    AudioHandler::GetSingleton()->PlaySound("P2");
                }
            });

        KKTween::delayedCall(1.0, [=, this]()
            {
                int nDraw3Count = 0;
                for (int i = 0; i < this->tableCardDraw3Go.Num(); i++)
                {
                    nDraw3Count = nDraw3Count + 1;

                    auto mCardItem = this->tableCardDraw3Go[i];
                    auto fromPos = this->GetCardNodeSendPokerPos();
                    auto toPos = this->GetCardNodeDraw3Pos(i);

                    auto mTween = KKTweenExtentions::UMG_MoveLocal_SlotPos(mCardItem, toPos, 0.3, KKTween::EaseType::easeOutQuad)->SetOnCompleteFunc(
                        [=, this]()
                        {
                            if (mCardItem->orTurnOverStateIsTrue())
                            {
                                mCardItem->PlayTurnOverAni();
                            }
                        });
                }

                if (nDraw3Count > 0)
                {
                    AudioHandler::GetSingleton()->PlaySound("P2");
                }
            });
    }
    else
    {
        this->RefreshAllPokerState();
    }

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
    UEHelper::StartTimer(this, this->mTimer);
    this->UpdateGameMode();
    //this->ResetRemainHintCount();
    this->onAddScore_InitParam();
    this->GameWinAniMgr->DestroyAniNode();
    this->DoWhenSet_FastGame();
}

void UMainUIWidget::RefreshAllPokerState()
{
    for (auto v : this->mSendCardListGo)
    {
        auto mCardItem = v;
        UMGHelper::SetChildLastZOrder(mCardItem);
        UMGHelper::SetSlotPos(mCardItem, this->GetCardNodeSendPokerPos());
        mCardItem->Refresh();
        mCardItem->SetEventTriggerState(false);
        mCardItem->Show();
    }

    for (int i = 0; i < this->tableCardDraw3Go.Num(); i++)
    {
        auto mCardItem = this->tableCardDraw3Go[i];
        UMGHelper::SetAsFirstChildIndex(mCardItem);
        UMGHelper::SetSlotPos(mCardItem, this->GetCardNodeDraw3Pos(i));
        mCardItem->Refresh();
        mCardItem->SetEventTriggerState(i == 1);
        if (i <= 2)
        {
            mCardItem->Show();
        }
        else
        {
            mCardItem->Hide();
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < this->tableCardNode4AGo[i].Num(); i++)
        {
            auto mCardItem = this->tableCardNode4AGo[i][j];
            UMGHelper::SetChildLastZOrder(mCardItem);
            UMGHelper::SetSlotPos(mCardItem, this->GetCardNode4APos(i));
            mCardItem->Refresh();
            mCardItem->Show();
            mCardItem->SetEventTriggerState(j == this->tableCardNode4AGo[i].Num());
        }
    }

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
        {
            auto mCardItem = this->tableCardNodeTop7Go[i][j];
            UMGHelper::SetChildLastZOrder(mCardItem);
            UMGHelper::SetSlotPos(mCardItem, this->GetCardNodeTop7Pos(i, j));
            mCardItem->Show();
            mCardItem->Refresh();
            mCardItem->SetEventTriggerState(mCardItem->orTurnOverStateIsTrue());
        }
        this->DoTop7ReSizeHeightAni(i);
    }

}

void UMainUIWidget::NewGameBegin_ForNormal(bool bForceNewGame)
{
    this->nGameMode = (int32)SolitaireGameMode::Normal;
    if (bForceNewGame)
    {
        this->NewGameBegin(false);
    }
    else
    {
        if (RecordStepDataHandler::GetSingleton()->GetData()->nGameMode == this->nGameMode &&
            RecordStepDataHandler::GetSingleton()->orGameEnd() == false)
        {
            //--牌局未改变
        }
        else
        {
            auto nLastRecordData = AllRecordDataHandler::GetSingleton()->RemoveAndGetLastDifferentGameModeRecordTable(this->nGameMode);
            if (nLastRecordData)
            {
                RecordStepDataHandler::GetSingleton()->InitStepRecordFromOther(*nLastRecordData);
                this->RecoverGame();
            }
            else
            {
                this->NewGameBegin(false);
            }
        }
    }
}

void UMainUIWidget::NewGameBegin_ForChallenge()
{
    this->nGameMode = SolitaireGameMode::DailyChallenge;
    this->NewGameBegin(false);
}

void UMainUIWidget::NewGameBegin_ForRank()
{
    this->nGameMode = SolitaireGameMode::Rank;
    this->NewGameBegin(false);
}

void UMainUIWidget::NewGameBegin_ForRePlay()
{
    this->NewGameBegin(true);
}


void UMainUIWidget::NewGameBegin(bool bRePlay)
{
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget NewGameBegin 000000"));
    // ----------------------------------------------广告--------------------------------------------
    //if (ThemeSolitaire.Config:orTriggerBannerAds() then
    //     if not GoogleAdsHandler.bShowBannerAds then
    //         GoogleAdsHandler : Show_BannerAds(true)
    //         end
    //         end

    //         if ThemeSolitaire.Config : orTriggerRewardedInterstitialAds() then
    //             GoogleAdsHandler : Show_RewardedInterstitialAds()
    //             end

    //--------------------------------------对上把数据 记录整理--------------------------------------------
    if (DataCenter::GetSingleton()->GetData()->nTotalGameCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("UMainUIWidget NewGameBegin DataCenter::GetSingleton()->GetData()->nTotalGameCount > 0"));
        RecordStepDataHandler::GetSingleton()->GetData()->bWin = true;
        if (RecordStepDataHandler::GetSingleton()->GetData()->nGameMode == SolitaireGameMode::Normal)
        {
            if (RecordStepDataHandler::GetSingleton()->GetData()->bWin)
            {
                DataCenter::GetSingleton()->AddNomalModeTotalWinCount();
            }
            DataCenter::GetSingleton()->AddDifficultLayerWinResult(RecordStepDataHandler::GetSingleton()->GetData()->bWin);
        }

        if (RecordStepDataHandler::GetSingleton()->GetData()->bWin)
        {
            DataCenter::GetSingleton()->AddTotalWinGameCount();
        }

        StatisticDataHandler::GetSingleton()->onGameStatistic(
            RecordStepDataHandler::GetSingleton()->GetData()->bWin,
            RecordStepDataHandler::GetSingleton()->GetData()->nMoveCount,
            RecordStepDataHandler::GetSingleton()->GetData()->nTime,
            RecordStepDataHandler::GetSingleton()->GetData()->nScore);

        if (RecordStepDataHandler::GetSingleton()->GetData()->bWin)
        {
            if (DataCenter::GetSingleton()->GetData()->nNomalModeTotalWinCount == 2 && DataCenter::GetSingleton()->GetData()->bFastGame == false)
            {
                KKTween::delayedCall(1.0, []()
                    {
                        //ThemeSolitaire.Guide_FastPlayView:Show()
                    });
            };

            if (DataCenter::GetSingleton()->GetData()->nNomalModeTotalWinCount == 4 && DataCenter::GetSingleton()->GetData()->nMusicIndex == 0)
            {
                KKTween::delayedCall(1.0, []()
                    {
                        // ThemeSolitaire.Guide_MusicOnView:Show()
                    });
            }
        }
    }

    // ----------------------------初始化数据--------------------------------------------
    DataCenter::GetSingleton()->data->nDifficultLayer = FMath::Clamp(DataCenter::GetSingleton()->data->nDifficultLayer, 1, 10);
    if (!bRePlay)
    {
        DataCenter::GetSingleton()->AddGameLevel();
    }

    DataCenter::GetSingleton()->data->nIQValue = 100;
    DataCenter::GetSingleton()->AddTotalGameCount();
    this->TellRobot_PlayerAlive();
    this->RecycleAndInitCardGo();
    this->bGameEnd = false;
    this->PokerItemParent->SetVisibility(ESlateVisibility::Visible);

    //--------------------------------------事件---------------------------------- -
    //this->PrintGameStartInfo();

    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget NewGameBegin 1111111"));
    TArray<int32> mSendCardList = {};
    if (bRePlay)
    {
        mSendCardList = this->mLastSendCardList;
    }
    else
    {
        if (this->nGameMode == SolitaireGameMode::DailyChallenge)
        {
            mSendCardList = CardHandler::GetSingleton()->GetInitCards_ForChallengeMode();
        }
        else if (this->nGameMode == SolitaireGameMode::Rank)
        {
            mSendCardList = CardHandler::GetSingleton()->GetInitCards_ForRankMode();
        }
        else if (this->nGameMode == SolitaireGameMode::Trip)
        {
           
        }
        else
        {
            mSendCardList = CardHandler::GetSingleton()->GetInitCards_ForNormalMode();
        }
    }

    this->mLastSendCardList = mSendCardList;

    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget NewGameBegin 222222: %d %d"), mSendCardList.Num(), this->nGameMode);
    ensureMsgf(mSendCardList.Num() == 52, TEXT("%d"), mSendCardList.Num());
    ensureMsgf(this->mSendCardListGo.Num() == 52, TEXT("%d"), this->mSendCardListGo.Num());
    RecordStepDataHandler::GetSingleton()->InitStepRecord(this->nGameMode, mSendCardList);
    //CollectPokerTaskDataHandler::GetSingleton()->SetGameBeginPokerId(mSendCardList);

    for (int32 i = 0; i < mSendCardList.Num(); i++)
    {
        int32 nPokerId = mSendCardList[i];
        this->mSendCardListGo[i]->SetPokerId(nPokerId);
        this->mSendCardListGo[i]->Show();
        this->mSendCardListGo[i]->SetTurnOverState(false);
        this->mSendCardListGo[i]->Refresh();
        this->mSendCardListGo[i]->SetEventTriggerState(false);
        UMGHelper::SetSlotPos(this->mSendCardListGo[i], this->tranFaPaiPos);
        UMGHelper::SetZOrder(this->mSendCardListGo[i], 0);
    }

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
    //--------------------------- 做发牌动画---------------------------------
    AudioHandler::GetSingleton()->PlaySound(TEXT("start_new"));
    
    for (int32 i = 0; i < 7; i++)
    {
        for (int32 j = 0; j <= i; j++)
        {
            int32 nTopIndex = i;
            auto mCardItem = this->mSendCardListGo.Pop();

            int32 nHeightIndex = this->tableCardNodeTop7Go[nTopIndex].Num();
            bool bTurnOverState = nHeightIndex == nTopIndex;
            if (bTurnOverState)
            {
                mCardItem->SetTurnOverState(true);
            }

            FVector2D from = UMGHelper::GetSlotPos(mCardItem);
            FVector2D to = this->GetCardNodeTop7MaxHeightPos(nTopIndex);;
            this->tableCardNodeTop7Go[nTopIndex].Add(mCardItem);

            UMGHelper::SetChildLastZOrder(mCardItem);
            KKTweenExtentions::UMG_MoveLocal_SlotPos(mCardItem, to, 0.3)->SetDelay(0.05 * j)->SetOnStartFunc([=,this]()
            {
                
            })->SetOnCompleteFunc([=, this]()
                {
                    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
                    if (bTurnOverState)
                    {
                        mCardItem->PlayTurnOverAni();
                        mCardItem->SetEventTriggerState(true);
                    }
                });
        }
    }

    this->GameWinAniMgr->DestroyAniNode();
    this->UpdateGameMode();
    this->onAddScore_InitParam();
    UEHelper::StartTimer(this, this->mTimer);

    this->TellRobot_PlayerAlive();
    //this->ResetRemainHintCount()
    KKTween::delayedCall(this, 1.0, [this]()
        {
            this->DoWhenSet_FastGame();
            /*if (ThemeSolitaire.Config.bRobotTest)
             {
                 this->StartRobotPlay();
             }*/
        });
}

//--------------------------------玩家操作-------------------------------------------- -
void UMainUIWidget::TellRobot_PlayerAlive()
{
    this->fRobotThinkingTime = 0.0;
}

void UMainUIWidget::OnClickChuPai()
{
    if (this->mSendCardListGo.Num() == 0 && this->tableCardDraw3Go.Num() <= 1)
    {
        return;
    }

    if (GameConst::bRobotTest)
    {
        DataCenter::GetSingleton()->data->nDrawCount = FMath::RandRange(1, 3);
    }

    RecordStepDataHandler::GetSingleton()->AddMoveCount();
    auto data = DataCenter::GetSingleton()->data;
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget OnClickChuPai: %d, %d, %d"), this->mSendCardListGo.Num(), this->tableCardDraw3Go.Num(), data->nDrawCount);

    if (this->mSendCardListGo.Num() == 0)
    {
        int32 nAniIndex = 0;
        while (this->tableCardDraw3Go.Num() > 0)
        {
            nAniIndex = nAniIndex + 1;
            auto mCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go);
            this->mSendCardListGo.Insert(mCardItem, 0);

            mCardItem->SetEventTriggerState(false);
            mCardItem->SetTurnOverState(false);
            mCardItem->PlayTurnOverAni();
            mCardItem->Show();

            FVector2D from = UMGHelper::GetSlotPos(mCardItem);
            FVector2D to = this->tranFaPaiPos;
            this->OnClickDraw3Move(mCardItem, from, to, false);
        }

        auto mOpStepItemData = RecordStepDataHandler::GetSingleton()->GetOpStepItemDefaultData();
        mOpStepItemData.fromPosTypeInfo = { SolitairePokerPosType::Draw3Pos, 0, 0 };
        mOpStepItemData.toPosTypeInfo = { SolitairePokerPosType::SendPokerPos, 0, 0 };
        RecordStepDataHandler::GetSingleton()->AddStepRecord(mOpStepItemData);

        AudioHandler::GetSingleton()->PlaySound("P4");
        KKTween::delayedCall(this, 0.5, [this]()
            {
                this->DoWhenSet_FastGame();
            });
    }
    else
    {
        int32 nMaxDrawCount = FMath::Min(data->nDrawCount, this->mSendCardListGo.Num());
        for (int32 i = 0; i < nMaxDrawCount; i++)
        {
            auto mCardItem = TArrayExtentions::Remove(this->mSendCardListGo);
            this->tableCardDraw3Go.Insert(mCardItem, 0);
            UMGHelper::SetChildLastZOrder(mCardItem);
            mCardItem->SetTurnOverState(true);
            mCardItem->PlayTurnOverAni();
        }

        auto mOpStepItemData = RecordStepDataHandler::GetSingleton()->GetOpStepItemDefaultData();
        mOpStepItemData.fromPosTypeInfo = { SolitairePokerPosType::SendPokerPos, nMaxDrawCount, 0 };
        mOpStepItemData.toPosTypeInfo = { SolitairePokerPosType::Draw3Pos, 0, 0 };
        RecordStepDataHandler::GetSingleton()->AddStepRecord(mOpStepItemData);

        this->nRemainHintCount_InDraw3AndSendCardList -= nMaxDrawCount;
        this->RefreshDrawZone();

        if (DataCenter::GetSingleton()->data->nDrawCount == 1)
        {
            AudioHandler::GetSingleton()->PlaySound("P1");
        }
        else if (DataCenter::GetSingleton()->data->nDrawCount == 2)
        {
            AudioHandler::GetSingleton()->PlaySound("P2");
        }
        else if (DataCenter::GetSingleton()->data->nDrawCount == 3)
        {
            AudioHandler::GetSingleton()->PlaySound("P3");
        }
    }
}

void UMainUIWidget::RefreshDrawZone()
{
    int32 nMaxCount = FMath::Min(3, this->tableCardDraw3Go.Num());
    for (int32 i = 0; i < nMaxCount; i++)
    {
        UPokerItemWidget* mCardItem = this->tableCardDraw3Go[i];
        mCardItem->Show();

        FVector2D from = UMGHelper::GetSlotPos(mCardItem);
        FVector2D to = this->GetCardNodeDraw3Pos(i);
        mCardItem->SetEventTriggerState(i == 0);
        this->OnClickDraw3Move(mCardItem, from, to, false);
    }

    for (int32 i = 3; i < this->tableCardDraw3Go.Num(); i++)
    {
        auto mCardItem = this->tableCardDraw3Go[i];
        mCardItem->SetEventTriggerState(false);

        FVector2D from = UMGHelper::GetSlotPos(mCardItem);
        FVector2D to = this->GetCardNodeDraw3Pos(i);
        this->OnClickDraw3Move(mCardItem, from, to, false, [=]()
            {
                mCardItem->Hide();
            });
    }
}

void UMainUIWidget::OnClickToMovePokerPos(UPokerItemWidget* mCardItem)
{
    auto posTypeInfo = this->GetPokerPosType(mCardItem);
    if (posTypeInfo[0] == SolitairePokerPosType::Draw3Pos)
    {
        for (int32 i = 0; i < 4; i++)
        {
            if (this->orCanIn4A(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::A4Pos, i);
                return;
            }
        }
        for (int32 i = 0; i < 7; i++)
        {
            if (this->orCanInNode7(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::Top7Pos, i);
                return;
            }
        }
    }
    else if (posTypeInfo[0] == SolitairePokerPosType::A4Pos)
    {
        for (int32 i = 0; i < 7; i++)
        {
            if (this->orCanInNode7(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::Top7Pos, i);
                return;
            }
        }
    }
    else if (posTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        for (int32 i = 0; i < 4; i++)
        {
            if (this->orCanIn4A(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::A4Pos, i);
                return;
            }
        }

        for (int32 i = 0; i < 7; i++)
        {
            if (this->orCanInNode7(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::Top7Pos, i);
                return;
            }
        }
    }
    else
    {
        check(false);
    }

    AudioHandler::GetSingleton()->PlaySound("CardShake");
    auto mSelectCardItem = this->GetSelectCardItemList(mCardItem);
    for (auto v : mSelectCardItem)
    {
        v->DoShakeAni();
    }
}

void UMainUIWidget::OnDragBegin(UPokerItemWidget* mDragCardItem)
{
    auto mCardItemList = this->GetSelectCardItemList(mDragCardItem);
    for (int32 i = 0; i < mCardItemList.Num(); i++)
    {
        auto mCardItem = mCardItemList[i];
        if (this->mapCardItemTween.Contains(mCardItem))
        {
            this->mapCardItemTween[mCardItem].Cancel();
            this->mapCardItemTween.Remove(mCardItem);
        }
        UMGHelper::SetChildLastZOrder(mCardItem);
    }
}

void UMainUIWidget::OnDrag(UPokerItemWidget* mDragCardItem)
{
    auto mCardItemList = this->GetSelectCardItemList(mDragCardItem);
    auto oriPosTypeInfo = this->GetPokerPosType(mDragCardItem);
    if (oriPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        int32 nTop7Index = oriPosTypeInfo[1];
        int32 nIndex = oriPosTypeInfo[2];
        FVector2D oriPos = UMGHelper::GetSlotPos(mDragCardItem);
        for (int32 i = 0; i < mCardItemList.Num(); i++)
        {
            auto mCardItem = mCardItemList[i];
            if (i > 0)
            {
                FVector2D Pos = oriPos + FVector2D(0, i * N_TOP7_GAP_HEIGHT);
                UMGHelper::SetSlotPos(mCardItem, Pos);
            }
        }
    }
}

void UMainUIWidget::OnDragEndToMovePokerPos(UPokerItemWidget* mCardItem)
{
    auto mCardItemPos = UMGHelper::GetSlotPos(mCardItem);
    float mCardItemPosY = mCardItemPos.Y;

    auto orInZone = [=](FVector2D middlePos)
        {
            if (mCardItemPos.X >= middlePos.X - 80 && mCardItemPos.X <= middlePos.X + 80 &&
                mCardItemPos.Y >= middlePos.Y - 80 && mCardItemPos.Y <= middlePos.Y + 80)
            {
                return true;
            }

            return false;
        };

    bool bIn4APos = mCardItemPosY < this->tableCardNode4APos[0].Y + 75;
    if (bIn4APos)
    {
        for (int32 i = 0; i < 4; i++)
        {
            FVector2D middlePos = this->GetCardNode4APos(i);
            if (orInZone(middlePos))
            {
                if (this->orCanIn4A(i, mCardItem))
                {
                    this->LockTargetToMove(mCardItem, SolitairePokerPosType::A4Pos, i);
                    return;
                }
                break;
            }
        }
    }
    else
    {
        int32 nTop7Index = -1;
        for (int32 i = 0; i < 7; i++)
        {
            FVector2D middlePos = this->GetCardNodeTop7NextMaxHeightPos(i);
            if (orInZone(middlePos))
            {
                if (this->orCanInNode7(i, mCardItem))
                {
                    this->LockTargetToMove(mCardItem, SolitairePokerPosType::Top7Pos, i);
                    return;
                }
                break;
            }
        }
    }

    this->LockTargetToMove(mCardItem, -1, 0);
}

void UMainUIWidget::LockTargetToMove(UPokerItemWidget* mCardItem, int32 nPosType, int32 nIndex = 0)
{
    auto oriPosTypeInfo = this->GetPokerPosType(mCardItem);
    auto mCardItemList = this->GetSelectCardItemList(mCardItem);

    bool bToOtherPos = false;
    if (nPosType == SolitairePokerPosType::A4Pos)
    {
        int32 n4AIndex = nIndex;
        auto& mListCardNode4AGo = this->tableCardNode4AGo[n4AIndex];
        if (mListCardNode4AGo.Num() > 0)
        {
            mListCardNode4AGo[mListCardNode4AGo.Num() - 1]->SetEventTriggerState(false);
        }

        this->SetDragEndRemoveSelfFromArray(mCardItem);
        mListCardNode4AGo.Add(mCardItem);
        bToOtherPos = true;

        if (mListCardNode4AGo.Num() < 8)
        {
            AudioHandler::GetSingleton()->PlaySound(FString::Format(TEXT("receive_card_{0}"), { mListCardNode4AGo.Num() }));
        }
        else
        {
            AudioHandler::GetSingleton()->PlaySound("receive_card_8");
        }
    }
    else if (nPosType == SolitairePokerPosType::Top7Pos)
    {
        int32 nTop7Index = nIndex;
        auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTop7Index];
        int32 nNowHeight = mListCardNodeTop7Go.Num();
        bToOtherPos = true;

        auto tableArray = this->SetDragEndRemoveSelfFromArray(mCardItem);
        this->InsertArrayToTop7Go(nTop7Index, tableArray);
        AudioHandler::GetSingleton()-> PlaySound("P2");
    }

    TArray<int32> targetPosTypeInfo = {};
    if (bToOtherPos)
    {
        RecordStepDataHandler::GetSingleton()->AddMoveCount();
        targetPosTypeInfo = this->GetPokerPosType(mCardItem);

        ensureMsgf(oriPosTypeInfo.Num() == 3, TEXT("oriPosTypeInfo.Num() : %d"), oriPosTypeInfo.Num());
        ensureMsgf(targetPosTypeInfo.Num() == 3, TEXT("targetPosTypeInfo.Num() : %d"), targetPosTypeInfo.Num());

        auto mOpStepItemData = RecordStepDataHandler::GetSingleton()->GetOpStepItemDefaultData();
        mOpStepItemData.fromPosTypeInfo = oriPosTypeInfo;
        mOpStepItemData.toPosTypeInfo = targetPosTypeInfo;
        RecordStepDataHandler::GetSingleton()->AddStepRecord(mOpStepItemData);

        this->onAddScore();
        this->onAddIQ();
        this->DoWhenSet_FastGame();

        if (oriPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            this->DoTop7ReSizeHeightAni(oriPosTypeInfo[1]);
        }

        if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            this->DoTop7ReSizeHeightAni(targetPosTypeInfo[1]);
        }
    }

    for (auto v : mCardItemList)
    {
        auto mTempCardItem = v;
        auto cardPosTypeInfo = this->GetPokerPosType(v);
        FVector2D from = UMGHelper::GetSlotPos(v);
        FVector2D to = this->GetPosByPosTypeInfo(cardPosTypeInfo);

        this->OnDragEndMove(mTempCardItem, from, to, false, [=,this]()
            {
                if (targetPosTypeInfo.Num() > 0)
                {
                    if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
                    {
                        this->PlayToA4EffectAni(targetPosTypeInfo[2], mTempCardItem);
                    }
                }
            });
    }

    if ((!this->bGameEnd) && oriPosTypeInfo[0] != SolitairePokerPosType::A4Pos)
    {
        if (this->tween_OnFastGameToResultA4.IsValid())
        {
            this->tween_OnFastGameToResultA4.Cancel();
        }

        this->tween_OnFastGameToResultA4 = KKTween::GetHandle(KKTween::delayedCall(this, 0.35, [this]()
            {
                this->OnFastGameToResultA4();
            }));
    }

    this->DoActionEnd();
}

//------------------------------------------------判断输赢------------------------------------------------------------ -
bool UMainUIWidget::JudgeWin()
{
    bool bWin = true;
    if (DataCenter::GetSingleton()->data->bAutoComplete)
    {
        for (int32 i = 0; i < 7; i++)
        {
            for (auto v : this->tableCardNodeTop7Go[i])
            {
                if (!v->orTurnOverStateIsTrue())
                {
                    bWin = false;
                    break;
                }
            }
        }
    }
    else
    {
        for (int32 i = 0; i < 4; i++)
        {
            if (this->tableCardNode4AGo[i].Num() != 13)
            {
                bWin = false;
                break;
            }
        }
    }

    return bWin;
}

//---------------------------------------------------------- - 播放胜利动画--------------------------------------------------------------------------
void UMainUIWidget::DoActionEnd()
{
    if (this->nGameMode == SolitaireGameMode::Trip)
    {
        this->UpdateGameMode();
    }
    this->CheckGameEnd();
}

void UMainUIWidget::CheckGameEnd()
{
    if (this->bGameEnd)
    {
        return;
    }

    if (this->nGameMode == SolitaireGameMode::Rank)
    {
        if (this->JudgeWin())
        {
            this->bGameEnd = true;
            this->DoA4AllDataOp();
            UEHelper::PauseTimer(this, this->mTimer);
            this->TellRobot_PlayerAlive();

            //ThemeSolitaire.StageRewardHandler:AddWin();
            //ThemeSolitaire.DailyRankDataHandler:SubmitScore();

            RecordStepDataHandler::GetSingleton()->SetLoseWin(true);
            DataCenter::GetSingleton()->AddCoinCount(GameConst::nFightResultWinGetCoinsCount);
            KKTween::delayedCall(this, 0.5, [this]()
                {
                    this->PlayWinAni();
                });
        }
    }
    else if (this->nGameMode == SolitaireGameMode::Trip)
    {
        /* if (ThemeSolitaire.TripHandler : JudgeWin())
         {
             this->bGameEnd = true
                 this->mTimer : Stop()
                 this->DoA4AllDataOp()
                 this->TellRobot_PlayerAlive()

                 ThemeSolitaire.RecordStepDataHandler : SetLoseWin(true)
                 ThemeSolitaire.DataCenter : AddCoinCount(ThemeSolitaire.Config.nFightResultWinGetCoinsCount)
                 ThemeSolitaire.TripHandler : FinishTripMission()

                 TweenMgr : delayedCallWithGo(this->transform, 0.5, function()
                     ThemeSolitaire.GameTripWinView:Show()
                     end)
                 end
         }*/
    }
    else if (this->nGameMode == SolitaireGameMode::DailyChallenge)
    {
        if (this->JudgeWin())
        {
            this->bGameEnd = true;
            this->DoA4AllDataOp();
            UEHelper::PauseTimer(this, this->mTimer);
            this->TellRobot_PlayerAlive();

            //ThemeSolitaire.StageRewardHandler : AddWin();
            DataCenter::GetSingleton()->AddCoinCount(GameConst::nFightResultWinGetCoinsCount);
            RecordStepDataHandler::GetSingleton()->SetLoseWin(true);

            KKTween::delayedCall(this, 0.5, [this]()
                {
                    this->PlayWinAni();
                });
        }
    }
    else if (this->nGameMode == SolitaireGameMode::Normal)
    {
        if (this->JudgeWin())
        {
            this->bGameEnd = true;
            this->DoA4AllDataOp();
            UEHelper::PauseTimer(this, this->mTimer);
            this->TellRobot_PlayerAlive();

            //ThemeSolitaire.StageRewardHandler : AddWin();
            DataCenter::GetSingleton()->AddCoinCount(GameConst::nFightResultWinGetCoinsCount);
            RecordStepDataHandler::GetSingleton()->SetLoseWin(true);

            if (this->GameWinAniMgr)
            {
                this->GameWinAniMgr->Show();
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("this->GameWinAniMgr == null"));
            }

            KKTween::delayedCall(this, 0.5,
                [this]()
                {
                    this->PlayWinAni();
                });
        }
    }
    else
    {
        ensureMsgf(false, TEXT("%d"), this->nGameMode);
    }
}

void UMainUIWidget::DoA4AllDataOp(TFunction<void()> finishFunc)
{
    TArray<UPokerItemWidget*> tableGo = {};
    for (int32 i = 0; i < 7; i++)
    {
        for (auto v : this->tableCardNodeTop7Go[i])
        {
            tableGo.Add(v);
        }
        this->tableCardNodeTop7Go[i] = {};
    }

    for (auto v : this->tableCardDraw3Go)
    {
        tableGo.Add(v);
    }

    this->tableCardDraw3Go = {};

    for (auto v : this->mSendCardListGo)
    {
        tableGo.Add(v);
    }
    this->mSendCardListGo = {};

    for (auto v : tableGo)
    {
        v->SetEventTriggerState(false);
    }
    
    this->tableFinalA4AniCardItem = {};
    for (int32 i = 0; i < 13; i++)
    {
        for (int32 j = 3; j >= 0; j--)
        {
            for (int32 k = 0; k < tableGo.Num(); k++)
            {
                auto mCardItem = tableGo[k];
                if (this->orCanIn4A(j, mCardItem))
                {
                    TArrayExtentions::Remove(tableGo, k);
                    this->tableCardNode4AGo[j].Add(mCardItem);
                    RecordStepDataHandler::GetSingleton()->AddScore(5);
                    RecordStepDataHandler::GetSingleton()->AddMoveCount();
                    this->tableFinalA4AniCardItem.Add(mCardItem);
                    break;
                }
            }
        }
    }
    
}

void UMainUIWidget::PlayWinAni()
{
    if (DataCenter::GetSingleton()->data->bAutoComplete)
    {
        this->AutoShouPai([this]()
            {
                this->DoWinAnimation();
            });
    }
    else
    {
        this->DoWinAnimation();
    }
}

void UMainUIWidget::AutoShouPai(TFunction<void()> finishFunc)
{
    auto mWinAudio = AudioHandler::GetSingleton()->PlaySound2("win_animation");
    for (int32 i = 0; i < this->tableFinalA4AniCardItem.Num(); i++)
    {
        auto mCardItem = this->tableFinalA4AniCardItem[i];
        auto fromPos = UMGHelper::GetSlotPos(mCardItem);
        auto toPos = this->GetPosByCardItem(mCardItem);
        int32 nAniIndex = i;
        KKTween::delayedCall(this, 0.03 * i,
            [=, this]()
            {
                mCardItem->SetTurnOverState(true);
                mCardItem->PlayTurnOverAni();
                UMGHelper::SetChildLastZOrder(mCardItem);
                KKTween::AddTween(this, 0.15,
                    [=](float fTimePercent)
                    {
                        UMGHelper::SetSlotPos(mCardItem, KKTween::EaseFunc::easeLinear(fromPos, toPos, fTimePercent));
                    },
                    [=, this]()
                    {
                        KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
                        if (nAniIndex == this->tableFinalA4AniCardItem.Num() - 1)
                        {
                            AudioHandler::GetSingleton()->StopSound(mWinAudio);
                            if (finishFunc.IsSet())
                            {
                                finishFunc();
                            }
                        }
                    });
            });
    }
}

void UMainUIWidget::DoWinAnimation()
{
    this->PokerItemParent->SetVisibility(ESlateVisibility::Hidden);
    auto mAudio = AudioHandler::GetSingleton()->PlaySound2("blast_bgm");
    this->GameWinAniMgr->PlayAni([=,this]()
        {
            AudioHandler::GetSingleton()->StopSound(mAudio);

            /*   if ThemeSolitaire.StageRewardHandler : CheckOrHaveReward() then
                   ThemeSolitaire.StageRewardView : Show(function()
                       this->ShowGameEndView()
                       end)
               else*/
            this->ShowGameEndView();
            //   end
        });
}

void UMainUIWidget::ShowGameEndView()
{
    if (this->nGameMode == SolitaireGameMode::DailyChallenge)
    {
        //ThemeSolitaire.DailyChallengeWinView : Show();
    }
    else if (this->nGameMode == SolitaireGameMode::Rank)
    {
        //ThemeSolitaire.DailyRankWinView : Show();
    }
    else
    {
        AKKUIMgr::GetSingleton()->Get<UGameWinWidget>()->Show();
    }
}

TArray<FVector2D> UMainUIWidget::GetTableA4WorldPos()
{
    TArray<FVector2D> tableWorldPos = {};
    for (int32 i = 0; i < 4; i++)
    {
        FVector2D pos = UMGHelper::GetRAbsolutePos(this->PokerItemParent, this->tableCardNode4APos[i]);
        tableWorldPos.Add(pos);
    }
    return tableWorldPos;
}

TArray<int32> UMainUIWidget::GetTableA4Color()
{
    TArray<int32> tableNowColor = { 1, 2, 3, 4 };
    TArray<int32> table4AColor = {0, 0, 0, 0};
    for (int32 i = 0; i < 4; i++)
    {
        ensureMsgf(this->tableCardNode4AGo[i].Num() == 13, TEXT("%d"), this->tableCardNode4AGo[i].Num());
        if (this->tableCardNode4AGo[i].Num() > 0)
        {
            int32 nColor = CardHandler::GetSingleton()->GetSubDigital(this->tableCardNode4AGo[i][0]->nPokerId);
            table4AColor[i] = nColor;
            int32 nRemoveIndex = tableNowColor.IndexOfByKey(nColor);
            TArrayExtentions::Remove(tableNowColor, nRemoveIndex);
        }
    }

    while (tableNowColor.Num() > 0)
    {
        int32 nColor = TArrayExtentions::Remove(tableNowColor);
        for (int32 i = 0; i < 4; i++)
        {
            if (table4AColor[i] == 0)
            {
                table4AColor[i] = nColor;
                break;
            }
        }
    }

    TArrayExtentions::PrintTArray(table4AColor, TEXT("table4AColor"));
    return table4AColor;
}

//---------------------------------------------------------- - 移除插入操作--------------------------------------------------------------------------
TArray<UPokerItemWidget*> UMainUIWidget::RemoveArrayFromTop7Go(int32 nTopIndex, int32 nRemoveIndex)
{
    auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    TArray<UPokerItemWidget*> tableResult = {};
    while (mListCardNodeTop7Go.Num() > nRemoveIndex)
    {
        auto mCardItem = TArrayExtentions::Remove(mListCardNodeTop7Go, nRemoveIndex);
        tableResult.Add(mCardItem);
    }
    return tableResult;
}

void UMainUIWidget::InsertArrayToTop7Go(int32 nTopIndex, const TArray<UPokerItemWidget*>& tableArray)
{
    auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    for (int32 i = 0; i < tableArray.Num(); i++)
    {
        mListCardNodeTop7Go.Add(tableArray[i]);
    }
}

TArray<UPokerItemWidget*> UMainUIWidget::SetDragEndRemoveSelfFromArray(UPokerItemWidget* mDragCardItem)
{
    auto posTypeInfo = this->GetPokerPosType(mDragCardItem);

    TArray<UPokerItemWidget*> tableResult = {};
    if (posTypeInfo[0] == SolitairePokerPosType::Draw3Pos)
    {
        auto mCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go, 0);
        tableResult.Insert(mCardItem, 0);
        this->RefreshDrawZone();
    }
    else if (posTypeInfo[0] == SolitairePokerPosType::A4Pos) //A4 -> A4
    {
        auto& mListCardNode4AGo = this->tableCardNode4AGo[posTypeInfo[1]];
        auto mCardItem = TArrayExtentions::Remove(mListCardNode4AGo);
        tableResult.Insert(mCardItem, 0);

        if (mListCardNode4AGo.Num() > 0)
        {
            mListCardNode4AGo[mListCardNode4AGo.Num() - 1]->SetEventTriggerState(true);
        }
    }
    else if (posTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        tableResult = this->RemoveArrayFromTop7Go(posTypeInfo[1], posTypeInfo[2]);

        auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[posTypeInfo[1]];
        if (mListCardNodeTop7Go.Num() > 0)
        {
            auto mCardItem = mListCardNodeTop7Go[mListCardNodeTop7Go.Num() - 1];
            if (!mCardItem->orTurnOverStateIsTrue())
            {
                mCardItem->SetTurnOverState(true);
                mCardItem->SetEventTriggerState(true);
                mCardItem->PlayTurnOverAni();
                //CollectPokerTaskDataHandler:Collect(mCardItem.nPokerId)
                KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateCollectState)->Broadcast(nullptr);
            }
        }
    }
    else
    {
        ensure(false);
    }

    if (tableResult.Num() > 0)
    {
        ensureMsgf(tableResult[0] == mDragCardItem, TEXT("Error Remove Poker"));
    }
    return tableResult;
}

void UMainUIWidget::UpdateAllPokerEventTriggerState()
{
    for (auto v : this->mSendCardListGo)
    {
        auto mCardItem = v;
        mCardItem->SetEventTriggerState(false);
    }

    for (int32 i = 0; i < this->tableCardDraw3Go.Num(); i++)
    {
        auto mCardItem = this->tableCardDraw3Go[i];
        mCardItem->SetEventTriggerState(i == 0);
    }

    for (int32 i = 0; i < 4; i++)
    {
        for (int32 j = 0; j < this->tableCardNode4AGo[i].Num(); j++)
        {
            auto mCardItem = this->tableCardNode4AGo[i][j];
            mCardItem->SetEventTriggerState(j == this->tableCardNode4AGo[i].Num() - 1);
        }
    }

    for (int32 i = 0; i < 7; i++)
    {
        for (int32 j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
        {
            auto mCardItem = this->tableCardNodeTop7Go[i][j];
            mCardItem->SetEventTriggerState(mCardItem->orTurnOverStateIsTrue());
        }
    }
}


//------------------------------------------------------游戏模式相关--------------------------------------------------------------------
void UMainUIWidget::UpdateGameMode()
{
    UE_LOG(LogTemp, Log, TEXT("this->nGameMode: %d"), this->nGameMode);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateGameModeState)->Broadcast(nullptr);
}

//----------------------------------------------------------- 增加分数--------------------------------------------------------------------------
std::tuple<bool, UPokerItemWidget*> UMainUIWidget::orThisStepTurnOverPokerIsTrue(int32 nStepIndex)
{
    for (int32 i = 0; i < 7; i++)
    {
        for (int32 j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
        {
            auto mCardItem = this->tableCardNodeTop7Go[i][j];
            if (mCardItem->nStepIndex_ForFirstShowPokerId == nStepIndex - 1)
            {
                return {true, mCardItem};
            }
        }
    }
    return { false, nullptr };
}

int32 UMainUIWidget::GetTop7HideCardCount()
{
    int32 nCount = 0;
    for (int32 i = 0; i < 7; i++)
    {
        for (int32 j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
        {
            auto mCardItem = this->tableCardNodeTop7Go[i][j];
            if (!mCardItem->orTurnOverStateIsTrue())
            {
                nCount = nCount + 1;
            }
        }
    }
    return nCount;
}

void UMainUIWidget::onAddScore_InitParam()
{
    this->nGetScore_nLastTop7HideCardCount = this->GetTop7HideCardCount();
}

void UMainUIWidget::onAddScore()
{
    int32 baseScore = 0;
    int32 addScore = 0;

    auto& tableOpStepItem = RecordStepDataHandler::GetSingleton()->GetData()->tableOpStepItem;
    auto& nLastOpInfo = tableOpStepItem[tableOpStepItem.Num() - 1];
    const auto& fromPosTypeInfo = nLastOpInfo.fromPosTypeInfo;
    const auto& toPosTypeInfo = nLastOpInfo.toPosTypeInfo;

    int32 nAddSumScore = 0;
    if (toPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
    {
        nAddSumScore += 5;
        int32 nLastIndex = tableOpStepItem.Num() - 2;
        int32 nContinueToA4StepCount = 0;
        while (nLastIndex >= 0)
        {
            ensureMsgf(nLastIndex >= 0 && nLastIndex < tableOpStepItem.Num(), TEXT("nLastIndex: %d"), nLastIndex);
            const auto& nTempLastOpInfo = tableOpStepItem[nLastIndex];
            nLastIndex--;

            ensureMsgf(nTempLastOpInfo.toPosTypeInfo.Num() == 3, TEXT("%d [%d] %d"), tableOpStepItem.Num(), nLastIndex, nTempLastOpInfo.toPosTypeInfo.Num());
            if (nTempLastOpInfo.toPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
            {
                nContinueToA4StepCount = nContinueToA4StepCount + 1;
            }
            else
            {
                break;
            }
        }

        nAddSumScore += 5 * nContinueToA4StepCount;
    }
    else if (toPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        if (fromPosTypeInfo[0] == SolitairePokerPosType::Draw3Pos)
        {
            nAddSumScore += 5;
        }

        int32 nLastIndex = tableOpStepItem.Num() - 2;
        int32 nContinueCount = 0;
        while (nLastIndex >= 0)
        {
            ensureMsgf(nLastIndex >= 0 && nLastIndex < tableOpStepItem.Num(), TEXT("nLastIndex: %d"), nLastIndex);
            const auto& nTempLastOpInfo = tableOpStepItem[nLastIndex];
            nLastIndex--;

            ensureMsgf(nTempLastOpInfo.toPosTypeInfo.Num() == 3, TEXT("%d [%d] %d"), tableOpStepItem.Num(), nLastIndex, nTempLastOpInfo.toPosTypeInfo.Num());
            if (nTempLastOpInfo.toPosTypeInfo[0] == SolitairePokerPosType::Top7Pos && 
                nTempLastOpInfo.nTureOverPokerId == nLastOpInfo.nTureOverPokerId)
            {
                nContinueCount++;
            }
            else
            {
                break;
            }
        }

        nAddSumScore += 5 * nContinueCount;
    }

    int32 nAddNewPokerCount = this->nGetScore_nLastTop7HideCardCount - this->GetTop7HideCardCount();
    if (nAddNewPokerCount > 0)
    {
        this->nGetScore_nLastTop7HideCardCount = this->GetTop7HideCardCount();
        nAddSumScore += nAddNewPokerCount * 5;
    }

    nLastOpInfo.nScore = nAddSumScore;
    RecordStepDataHandler::GetSingleton()->AddScore(nAddSumScore);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
}

//------------------------------------------------增加IQ----------------------------------------------------------
bool UMainUIWidget::orPosTypeInfoEqual(const TArray<int32>& info1, const TArray<int32>& info2)
{
    return info1[0] == info2[0] && info1[1] == info2[1] && info1[2] == info2[2];
}

bool UMainUIWidget::onIsLoopOp()
{
    const auto& tableOpStepItem = RecordStepDataHandler::GetSingleton()->GetData()->tableOpStepItem;
    const auto& nLastOpInfo = tableOpStepItem[tableOpStepItem.Num() - 1];

    int32 nBeginIndex = FMath::Max(0, tableOpStepItem.Num() - 10);
    for (int32 i = nBeginIndex; i < tableOpStepItem.Num() - 1; i++)
    {
        bool bSameOp = true;
        if (this->orPosTypeInfoEqual(nLastOpInfo.fromPosTypeInfo, tableOpStepItem[i].fromPosTypeInfo) == false ||
            this->orPosTypeInfoEqual(nLastOpInfo.toPosTypeInfo, tableOpStepItem[i].toPosTypeInfo) == false)
        {
            bSameOp = false;
            break;
        }

        if (bSameOp)
        {
            return true;
        }
    }

    return false;
}

void UMainUIWidget::onAddIQ()
{
    this->fIQTime = 0;
    if (this->onIsLoopOp())
    {
        DataCenter::GetSingleton()->AddIQValue(-1);
    }
    else
    {
        int32 nNowStepIndex = RecordStepDataHandler::GetSingleton()->GetData()->tableOpStepItem.Num() - 1;
        const auto& tableOpStepItem = RecordStepDataHandler::GetSingleton()->GetData()->tableOpStepItem;
        const auto& nLastOpInfo = tableOpStepItem[tableOpStepItem.Num() - 1];
        const auto& fromPosTypeInfo = nLastOpInfo.fromPosTypeInfo;
        const auto& toPosTypeInfo = nLastOpInfo.toPosTypeInfo;

        if (fromPosTypeInfo[0] != SolitairePokerPosType::A4Pos && toPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            DataCenter::GetSingleton()->AddIQValue(5);
        }
        else if (fromPosTypeInfo[0] != SolitairePokerPosType::A4Pos && toPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            auto [bTure, mCardItem] = this->orThisStepTurnOverPokerIsTrue(nNowStepIndex);
            if (bTure)
            {
                DataCenter::GetSingleton()->AddIQValue(5);
            }
        }
        else
        {
            DataCenter::GetSingleton()->AddIQValue(1);
        }
    }
}

//-------------------------------------------------- - 快速游戏 以及 A4Effect------------------------------------------------------------ -
void UMainUIWidget::PlayToA4EffectAni(int32 nIndex, UPokerItemWidget* mCardItem)
{
    if (!this->bInitA4Effect)
    {
        this->bInitA4Effect = true;
       // this->tableToA4EffectPool = {};
        for (int32 i = 0; i < 4; i++)
        {
            //auto goEffect = this->GetWidget("A4Effect/Color"..i).gameObject;
           /* local ItemPoolGenerator = require "Lua/SimpleFramework/ObjectPool/ItemPool"
                local SpineEffect = require "Lua.Effect.SpineEffect"
                this->tableToA4EffectPool[i] = ItemPoolGenerator : New(goEffect, SpineEffect, 0)
                end
            end

            local nPokerColor = CardHandler : GetSubDigital(mCardItem.nPokerId)
            local mSkeletonAnimation = this->tableToA4EffectPool[nPokerColor] : GetItem()
            this->SetRelativePos(mSkeletonAnimation, this->GetCardNode4APos(nIndex))
            mSkeletonAnimation : PlayOnce("animation", function()
                this->tableToA4EffectPool[nPokerColor]:RecycleItem(mSkeletonAnimation)
                end)*/
        }
    }
}

void UMainUIWidget::OnFastGameToResultA4()
{
    if (!DataCenter::GetSingleton()->data->bFastGame)
    {
        return;
    }

    for (int32 i = 0; i < 4; i++)
    {
        for (int32 j = 0; j < 7; j++)
        {
            const auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[j];
            if (mListCardNodeTop7Go.Num() > 0)
            {
                auto mCardItem = mListCardNodeTop7Go[mListCardNodeTop7Go.Num() - 1];
                if (mCardItem->bInDrag == false && this->orCanIn4A(i, mCardItem))
                {
                    this->OnDragBegin(mCardItem);
                    this->LockTargetToMove(mCardItem, SolitairePokerPosType::A4Pos, i);
                    return;
                }
            }
        }
    }
}

//------------------------------------ 相对位置计算 --------------------------------------------
float UMainUIWidget::GetTop7_Gap_Height(int32 nTopIndex)
{
    const auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    int32 nStartPosIndex = 5;
    int32 nZhengCount = 0;
    for(const auto& v : mListCardNodeTop7Go)
    {
        if (v->orTurnOverStateIsTrue())
        {
            nZhengCount = nZhengCount + 1;
        }
    }

    if (nZhengCount <= nStartPosIndex)
    {
        return UMainUIWidget::N_TOP7_GAP_HEIGHT;
    }
    else
    {
        float fPercent = (nZhengCount - nStartPosIndex) / (13 - nStartPosIndex);
        return UMainUIWidget::N_TOP7_GAP_HEIGHT * (1 - fPercent) + 40 * fPercent;
    }
}

FVector2D UMainUIWidget::GetCardNodeTop7MaxHeightPos(int32 nTopIndex)
{
    return this->GetCardNodeTop7Pos(nTopIndex, this->tableCardNodeTop7Go[nTopIndex].Num());
}

FVector2D UMainUIWidget::GetCardNodeTop7NextMaxHeightPos(int32 nTopIndex)
{
    return this->GetCardNodeTop7Pos(nTopIndex, this->tableCardNodeTop7Go[nTopIndex].Num() + 1);
}

FVector2D UMainUIWidget::GetCardNodeTop7Pos(int32 nTopIndex, int32 nNowCount)
{
    float nGapZhengDis = this->GetTop7_Gap_Height(nTopIndex);
    FVector2D oriPos = this->tableCardNodeTop7Pos[nTopIndex];
    const auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    float posY = oriPos.Y;
    for (int32 i = 0; i < nNowCount; i++)
    {
        if (i < mListCardNodeTop7Go.Num())
        {
            UPokerItemWidget* mCardItem = mListCardNodeTop7Go[i];
            if (mCardItem->orTurnOverStateIsTrue())
            {
                posY = posY + nGapZhengDis;
            }
            else
            {
                posY = posY + 20;
            }
        }
        else
        {
            posY = posY + nGapZhengDis;
        }
    }

    return FVector2D(oriPos.X, posY);
}

FVector2D UMainUIWidget::GetCardNode4APos(int32 nTopIndex)
{
    return this->tableCardNode4APos[nTopIndex];
}

FVector2D UMainUIWidget::GetCardNodeDraw3Pos(int32 nIndex)
{
    if (nIndex <= 2)
    {
        float fOffsetX = -44 * nIndex;
        return this->mCardNodeDraw3BeginPos + FVector2D(fOffsetX, 0);
    }
    else
    {
        float fOffsetX = -1 * (nIndex - 2);
        return this->GetCardNodeDraw3Pos(2) + FVector2D(fOffsetX, 0);
    }
}

FVector2D UMainUIWidget::GetCardNodeSendPokerPos()
{
    return this->tranFaPaiPos;
}

//-------------------------------------------------- 通用移动------------------------------------------------------------ -
void UMainUIWidget::OnClickDraw3Move(UPokerItemWidget* mCardItem, FVector2D fromPos, FVector2D toPos, bool bUndo, TFunction<void()> finishFunc)
{
    if (this->mapCardItemTween.Contains(mCardItem))
    {
        this->mapCardItemTween[mCardItem].Cancel();
        this->mapCardItemTween.Remove(mCardItem);
    }

    if (bUndo)
    {
        auto mTweenHandle = KKTween::GetHandle(KKTween::AddTween(this, 0.25,
            [=](float fTimePercent)
            {
                UMGHelper::SetSlotPos(mCardItem, KKTween::EaseFunc::easeInQuad(toPos, fromPos, fTimePercent));
            },
            [=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            }));

        this->mapCardItemTween.Add(mCardItem, mTweenHandle);
    }
    else
    {
        auto mTweenHandle = KKTween::GetHandle(KKTween::AddTween(this, 0.25,
            [=](float fTimePercent)
            {
                UMGHelper::SetSlotPos(mCardItem, KKTween::EaseFunc::easeOutQuad(fromPos, toPos, fTimePercent));
            },
            [=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            }));
        
        this->mapCardItemTween.Add(mCardItem, mTweenHandle);
    }
}

void UMainUIWidget::OnDragEndMove(UPokerItemWidget* mCardItem, FVector2D fromPos, FVector2D toPos, bool bUndo, TFunction<void()> finishFunc)
{
    if (this->mapCardItemTween.Contains(mCardItem))
    {
        this->mapCardItemTween[mCardItem].Cancel();
        this->mapCardItemTween.Remove(mCardItem);
    }

    if (bUndo)
    {
        auto mTween = KKTween::GetHandle(KKTween::AddTween(this, 0.35,
            [=](float fTimePercent)
            {
                UMGHelper::SetSlotPos(mCardItem, KKTween::EaseFunc::easeInQuad(toPos, fromPos, fTimePercent));
            },
            [=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            }));

        this->mapCardItemTween.Add(mCardItem, mTween);
    }
    else
    {
        auto mTween = KKTween::GetHandle(KKTween::AddTween(this, 0.35,
            [=](float fTimePercent)
            {
                UMGHelper::SetSlotPos(mCardItem, KKTween::EaseFunc::easeOutQuad(fromPos, toPos, fTimePercent));
            },
            [=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            }));

        this->mapCardItemTween.Add(mCardItem, mTween);
    }
}

void UMainUIWidget::DoTop7ReSizeHeightAni(int32 nTop7Index)
{
    const auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTop7Index];
    for (int32 i = 0; i < mListCardNodeTop7Go.Num(); i++)
    {
        auto mCardItem = mListCardNodeTop7Go[i];
        if (this->mapCardItemTween.Contains(mCardItem))
        {
            this->mapCardItemTween[mCardItem].Cancel();
            this->mapCardItemTween.Remove(mCardItem);
        }

        FVector2D fromPos = UMGHelper::GetSlotPos(mCardItem);
        FVector2D toPos = this->GetCardNodeTop7Pos(nTop7Index, i);
        auto mTween = KKTween::GetHandle(KKTween::AddTween(this, 0.2,
            [=](float fTimePercent)
            {
                UMGHelper::SetSlotPos(mCardItem, KKTween::EaseFunc::easeOutQuad(fromPos, toPos, fTimePercent));
            }));
        this->mapCardItemTween.Add(mCardItem, mTween);
    }
}

//-------------------------------------------零碎的方法---------------------------------

void UMainUIWidget::UpdateLeftHandState()
{
    //this->RefresPos();
    //this->RefreshAllPokerState();
}

bool UMainUIWidget::orHavePopView()
{
    UCanvasPanel* popCanvas = AKKUIMgr::GetSingleton()->GetRootWidget()->Layer_Popup;
    for (int32 i = 0; i < popCanvas->GetChildrenCount(); i++)
    {
        if(popCanvas->GetChildAt(i)->IsVisible())
        {
            return true;
        }
    }
    return false;
}

void UMainUIWidget::UpdateGameTimeState(bool bPause)
{
    if (bPause)
    {
        UEHelper::PauseTimer(this, this->mTimer);
    }
    else
    {
        UEHelper::StartTimer(this, this->mTimer);
    }
}

void UMainUIWidget::Set_FastGame()
{
    this->DoWhenSet_FastGame();
}

void UMainUIWidget::DoWhenSet_FastGame()
{
    if (DataCenter::GetSingleton()->data->bFastGame)
    {
        if (this->tableCardDraw3Go.Num() == 0)
        {
            this->OnClickChuPai();
        }
    }
}

void UMainUIWidget::Set_AutoHinted()
{
    if (DataCenter::GetSingleton()->data->bAutoHint)
    {
       //this->PlayHintAni();
    }
}

//---------------------------------------------------------- - 通用方法--------------------------------------------------------------------------
bool UMainUIWidget::orCanIn4A(int32 n4AIndex, UPokerItemWidget* mCardItem)
{
    auto targetPosTypeInfo = this->GetPokerPosType(mCardItem);
    if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        const auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[targetPosTypeInfo[1]];
        if (mListCardNodeTop7Go[mListCardNodeTop7Go.Num() - 1] != mCardItem)
        {
            return false;
        }
    }

    const auto& mListCardNode4A = this->tableCardNode4AGo[n4AIndex];
    if (mListCardNode4A.Num() == 0)
    {
        return CardHandler::GetSingleton()->GetDigital(mCardItem->nPokerId) == 1;
    }
    else
    {
        int32 nLastPokerId = mListCardNode4A[mListCardNode4A.Num() - 1]->nPokerId;
        auto [nLastNumber1, nLastNumber2] = CardHandler::GetSingleton()->GetTwoDigital(nLastPokerId);
        auto [nNumber1, nNumber2] = CardHandler::GetSingleton()->GetTwoDigital(mCardItem->nPokerId);
        if (nLastNumber1 + 1 == nNumber1 && nLastNumber2 == nNumber2)
        {
            return true;
        }
    }
    return false;
}

bool UMainUIWidget::orCanInNode7(int32 n7Index, UPokerItemWidget* mCardItem)
{
    const auto& tableCardNode7 = this->tableCardNodeTop7Go[n7Index];
    if (tableCardNode7.Num() == 0)
    {
        return CardHandler::GetSingleton()->GetDigital(mCardItem->nPokerId) == 13;
    }
    else
    {
        int32 nLastPokerId = tableCardNode7[tableCardNode7.Num() - 1]->nPokerId;
        if (CardHandler::GetSingleton()->GetDigital(nLastPokerId) - 1 == CardHandler::GetSingleton()->GetDigital(mCardItem->nPokerId) &&
            CardHandler::GetSingleton()->GetColor(nLastPokerId) != CardHandler::GetSingleton()->GetColor(mCardItem->nPokerId))
        {
            return true;
        }
    }

    return false;
}

FVector2D UMainUIWidget::GetPosByCardItem(UPokerItemWidget* mCardItem)
{
    auto oriPosTypeInfo = this->GetPokerPosType(mCardItem);
    return this->GetPosByPosTypeInfo(oriPosTypeInfo);
}

FVector2D UMainUIWidget::GetPosByPosTypeInfo(const TArray<int32>& nPosTypeInfo)
{
    SolitairePokerPosType nPosType = (SolitairePokerPosType)nPosTypeInfo[0];
    int32 nPosX = nPosTypeInfo[1];
    int32 nPosY = nPosTypeInfo[2];

    if (nPosType == SolitairePokerPosType::Draw3Pos)
    {
        return this->GetCardNodeDraw3Pos(nPosX);
    }
    else if (nPosType == SolitairePokerPosType::A4Pos)
    {
        return this->GetCardNode4APos(nPosX);
    }
    else if (nPosType == SolitairePokerPosType::Top7Pos)
    {
        return this->GetCardNodeTop7Pos(nPosX, nPosY);
    }
    else if (nPosType == SolitairePokerPosType::SendPokerPos)
    {
        return this->GetCardNodeSendPokerPos();
    }
    else
    {
        ensureMsgf(false, TEXT("GetPosByPosTypeInfo"));
    }

    return {};
}

TArray<UPokerItemWidget*> UMainUIWidget::GetSelectCardItemList(UPokerItemWidget* mCardItem)
{
    TArray<UPokerItemWidget*> tableCardItemList = {};
    TArray<int32> oriPosTypeInfo = this->GetPokerPosType(mCardItem);
    if (oriPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        int32 nTop7Index = oriPosTypeInfo[1];
        int32 nIndex = oriPosTypeInfo[2];
        for (int32 i = nIndex; i < this->tableCardNodeTop7Go[nTop7Index].Num(); i++)
        {
            auto mItem = this->tableCardNodeTop7Go[nTop7Index][i];
            tableCardItemList.Add(mItem);
        }
    }
    else
    {
        tableCardItemList.Add(mCardItem);
    }
    return tableCardItemList;
}

TArray<int32> UMainUIWidget::GetPokerPosType(UPokerItemWidget* mDragCardItem)
{
    int32 nIndex = this->tableCardDraw3Go.IndexOfByKey(mDragCardItem);
    if (nIndex != INDEX_NONE)
    {
        return { SolitairePokerPosType::Draw3Pos, nIndex, 0 };
    }

    for (int32 i = 0; i < 4; i++)
    {
        const auto& mListCardNode4AGo = this->tableCardNode4AGo[i];
        nIndex = mListCardNode4AGo.IndexOfByKey(mDragCardItem);
        if (nIndex != INDEX_NONE)
        {
            return { SolitairePokerPosType::A4Pos, i, nIndex };
        }
    }

    for (int32 i = 0; i < 7; i++)
    {
        const auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[i];
        nIndex = mListCardNodeTop7Go.IndexOfByKey(mDragCardItem);
        if (nIndex != INDEX_NONE)
        {
            return { SolitairePokerPosType::Top7Pos, i, nIndex };
        }
    }
    
    return { SolitairePokerPosType::SendPokerPos, 0, 0 };
}

//------------------------------------------------玩家记录------------------------------------------------------------ -
int32 UMainUIWidget::GetNowRecordStepIndex()
{
    auto [A, B] = RecordStepDataHandler::GetSingleton()->GetNowStepRecord();
    return A;
}

void UMainUIWidget::PlayRecordUndoAni()
{
    //this->CardHintEffectPool->Reset();
    if (!RecordStepDataHandler::GetSingleton()->orCanUndo())
    {
        return;
    }

    AudioHandler::GetSingleton()->PlaySound("undo");
    RecordStepDataHandler::GetSingleton()->AddMoveCount();
    DataCenter::GetSingleton()->AddIQValue(-2);
    auto [nStepIndex, nLastOpInfo] = RecordStepDataHandler::GetSingleton()->GetNowStepRecord();
    RecordStepDataHandler::GetSingleton()->AddUndoCount(1);

    if (nLastOpInfo->nScore > 0)
    {
        int nScore = nLastOpInfo->nScore;
        RecordStepDataHandler::GetSingleton()->AddScore(-nScore);
    }

    const auto& oriPosTypeInfo = nLastOpInfo->fromPosTypeInfo;
    const auto& targetPosTypeInfo = nLastOpInfo->toPosTypeInfo;
    auto fromPos = GetPosByPosTypeInfo(oriPosTypeInfo);
    auto toPos = GetPosByPosTypeInfo(targetPosTypeInfo);

    if (oriPosTypeInfo[0] == SolitairePokerPosType::SendPokerPos)
    {
        int nSendPokerCount = oriPosTypeInfo[1];

        int nCount = 0;
        while (nCount < nSendPokerCount)
        {
            nCount = nCount + 1;
            auto mCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go, 0);
            this->mSendCardListGo.Add(mCardItem);

            mCardItem->Show();
            UMGHelper::SetChildLastZOrder(mCardItem);
            mCardItem->SetEventTriggerState(false);
            mCardItem->SetTurnOverState(false);
            mCardItem->PlayTurnOverAni();

            toPos = this->GetCardNodeDraw3Pos(nCount);
            this->OnClickDraw3Move(mCardItem, fromPos, toPos, true);
        }
        this->RefreshDrawZone();
    }
    else if (oriPosTypeInfo[0] == SolitairePokerPosType::Draw3Pos)
    {
        auto mCardItem2 = this->tableCardDraw3Go[0];
        if (mCardItem2)
        {
            mCardItem2->SetEventTriggerState(false);
        }

        UPokerItemWidget* mCardItem = nullptr;
        if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            mCardItem = TArrayExtentions::Remove(this->tableCardNode4AGo[targetPosTypeInfo[1]]);
            this->tableCardDraw3Go.Insert(mCardItem, 0);
        }
        else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            int nTop7Index = targetPosTypeInfo[1];
            mCardItem = TArrayExtentions::Remove(this->tableCardNodeTop7Go[nTop7Index]);
            this->tableCardDraw3Go.Insert(mCardItem, 0);
            this->DoTop7ReSizeHeightAni(nTop7Index);
        }
        else if (targetPosTypeInfo[0] == SolitairePokerPosType::SendPokerPos)
        {
            ensure(this->tableCardDraw3Go.Num() == 0);
            while (this->mSendCardListGo.Num() > 0)
            {
                int nDraw3Index = this->mSendCardListGo.Num();
                auto mTempItem = TArrayExtentions::Remove(this->mSendCardListGo);
                mTempItem->SetTurnOverState(true);
                mTempItem->PlayTurnOverAni();
                UMGHelper::SetChildLastZOrder(mTempItem);
                this->tableCardDraw3Go.Insert(mTempItem, 0);

                fromPos = this->GetCardNodeDraw3Pos(nDraw3Index);
                this->OnClickDraw3Move(mTempItem, fromPos, toPos, true, [=]()
                    {
                        if (nDraw3Index <= 2)
                        {
                            mTempItem->Show();
                        }
                        else
                        {
                            mTempItem->Hide();
                        }
                    });
            }
        }
        else
        {
            ensure(false);
        }

        if (mCardItem)
        {
            this->OnDragBegin(mCardItem);
            this->OnDragEndMove(mCardItem, fromPos, toPos, true, [this]()
                {
                    this->RefreshDrawZone();
                });
        }
    }
    else if (oriPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
    {
        auto& mListCardNode4AGo = this->tableCardNode4AGo[oriPosTypeInfo[1]];
        if (mListCardNode4AGo.Num() > 0)
        {
            mListCardNode4AGo[mListCardNode4AGo.Num() - 1]->SetEventTriggerState(false);
        }

        UPokerItemWidget* mCardItem = nullptr;
        if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            auto& tableCardNode4AGo2 = this->tableCardNode4AGo[targetPosTypeInfo[1]];
            mCardItem = TArrayExtentions::Remove(tableCardNode4AGo2);
            mListCardNode4AGo.Add(mCardItem);
        }
        else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            int nTop7Index = targetPosTypeInfo[1];
            auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTop7Index];
            mCardItem = TArrayExtentions::Remove(mListCardNodeTop7Go);
            mListCardNode4AGo.Add(mCardItem);
            this->DoTop7ReSizeHeightAni(nTop7Index);
        }
        else
        {
            ensure(false);
        }

        this->OnDragBegin(mCardItem);
        this->OnDragEndMove(mCardItem, fromPos, toPos, true);
    }
    else if (oriPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        int oriTop7Index = oriPosTypeInfo[1];
        int oriIndex = oriPosTypeInfo[2];
        auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[oriTop7Index];
        if (oriIndex - 1 >= 0)
        {
            auto mCardItem2 = mListCardNodeTop7Go[oriIndex - 1];
            if (mCardItem2 and nStepIndex - 1 == mCardItem2->nStepIndex_ForFirstShowPokerId)
            {
                mCardItem2->SetTurnOverState(false);
                mCardItem2->PlayTurnOverAni();
                mCardItem2->SetEventTriggerState(false);
            }
        }

        fromPos = this->GetPosByPosTypeInfo(oriPosTypeInfo);
        UPokerItemWidget* mCardItem = nullptr;
        if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            bool bUseMagicWand = oriIndex != mListCardNodeTop7Go.Num();
            auto& mListCardNode4AGo = this->tableCardNode4AGo[targetPosTypeInfo[1]];
            mCardItem = TArrayExtentions::Remove(mListCardNode4AGo);
            mListCardNodeTop7Go.Insert(mCardItem, oriIndex);

            if (bUseMagicWand)
            {
                mCardItem->SetTurnOverState(false);
                mCardItem->PlayTurnOverAni();
                mCardItem->SetEventTriggerState(false);
            }

            if (mListCardNode4AGo.Num() > 0)
            {
                mListCardNode4AGo[mListCardNode4AGo.Num() - 1]->SetEventTriggerState(true);
            }
            else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
            {
                int nTargetTop7Index = targetPosTypeInfo[1];
                int nRemoveIndex = targetPosTypeInfo[2];
                auto taleResult = this->RemoveArrayFromTop7Go(nTargetTop7Index, nRemoveIndex);
                mCardItem = taleResult[0];
                this->InsertArrayToTop7Go(oriTop7Index, taleResult);
                this->DoTop7ReSizeHeightAni(nTargetTop7Index);
            }
            else
            {
                ensure(false);
            }

            this->DoTop7ReSizeHeightAni(oriTop7Index);
            this->OnDragBegin(mCardItem);
            auto mCardItemList = this->GetSelectCardItemList(mCardItem);
            for (auto v : mCardItemList)
            {
                auto mTempCardItem = v;
                auto cardPosTypeInfo = this->GetPokerPosType(v);
                auto to = UMGHelper::GetSlotPos(v);
                auto from = this->GetPosByPosTypeInfo(cardPosTypeInfo);
                this->OnDragEndMove(mTempCardItem, from, to, true);
            }
        }
    }

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
}

void UMainUIWidget::PlayRecordForwardAni()
{
    //this->CardHintEffectPool->Reset();
    if (!RecordStepDataHandler::GetSingleton()->orCanForward())
    {
        return;
    }

    AudioHandler::GetSingleton()->PlaySound("undo");
    RecordStepDataHandler::GetSingleton()->AddMoveCount();
    DataCenter::GetSingleton()->AddIQValue(-2);
    RecordStepDataHandler::GetSingleton()->AddUndoCount(-1);
    auto [nStepIndex, nLastOpInfo] = RecordStepDataHandler::GetSingleton()->GetNowStepRecord();

    if (nLastOpInfo->nScore > 0)
    {
        int nScore = nLastOpInfo->nScore;
        RecordStepDataHandler::GetSingleton()->AddScore(nScore);
    }

    const auto& oriPosTypeInfo = nLastOpInfo->fromPosTypeInfo;
    const auto& targetPosTypeInfo = nLastOpInfo->toPosTypeInfo;
    auto fromPos = this->GetPosByPosTypeInfo(oriPosTypeInfo);
    auto toPos = this->GetPosByPosTypeInfo(targetPosTypeInfo);

    if (oriPosTypeInfo[0] == SolitairePokerPosType::SendPokerPos)
    {
        int nSendPokerCount = oriPosTypeInfo[1];

        int nCount = 0;
        while (nCount < nSendPokerCount)
        {
            nCount++;
            auto mCardItem = TArrayExtentions::Remove(this->mSendCardListGo);
            this->tableCardDraw3Go.Insert(mCardItem, 0);

            UMGHelper::SetChildLastZOrder(mCardItem);
            mCardItem->SetTurnOverState(true, nStepIndex - 1);
            mCardItem->PlayTurnOverAni();
            this->OnClickDraw3Move(mCardItem, fromPos, toPos, false);
        }
        this->RefreshDrawZone();
    }
    else if (oriPosTypeInfo[0] == SolitairePokerPosType::Draw3Pos)
    {
        UPokerItemWidget* mCardItem = nullptr;
        if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            mCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go, 0);
            int nA4Index = targetPosTypeInfo[1];
            this->tableCardNode4AGo[nA4Index].Add(mCardItem);
        }
        else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            mCardItem = TArrayExtentions::Remove(this->tableCardDraw3Go, 0);
            int nTop7Index = targetPosTypeInfo[1];
            this->tableCardNodeTop7Go[nTop7Index].Add(mCardItem);
            this->DoTop7ReSizeHeightAni(nTop7Index);
        }
        else if (targetPosTypeInfo[0] == SolitairePokerPosType::SendPokerPos)
        {
            ensure(this->mSendCardListGo.Num() == 0);
            for (int i = 0; i < this->tableCardDraw3Go.Num(); i++)
            {
                auto mTempItem = this->tableCardDraw3Go[i];
                mTempItem->SetTurnOverState(false);
                mTempItem->PlayTurnOverAni();
                UMGHelper::SetChildLastZOrder(mTempItem);
                mTempItem->SetEventTriggerState(false);
                mTempItem->Show();
                this->mSendCardListGo.Add(mTempItem);

                fromPos = this->GetCardNodeDraw3Pos(i);
                this->OnClickDraw3Move(mTempItem, fromPos, toPos, false);
            }
            this->tableCardDraw3Go = {};
        }
        else
        {
            ensure(false);
        }

        if (mCardItem)
        {
            this->OnDragBegin(mCardItem);
            this->OnDragEndMove(mCardItem, fromPos, toPos, false, [this]()
                {
                    this->RefreshDrawZone();
                }
            );
        }
    }
    else if (oriPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
    {
        int nOriA4Index = oriPosTypeInfo[1];
        auto& mListCardNode4AGo = this->tableCardNode4AGo[nOriA4Index];
        auto mCardItem = TArrayExtentions::Remove(mListCardNode4AGo);
        if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            int nToA4Index = targetPosTypeInfo[1];
            auto& tableCardNode4AGo2 = this->tableCardNode4AGo[nToA4Index];
            if (tableCardNode4AGo2.Num() > 0)
            {
                tableCardNode4AGo2[mListCardNode4AGo.Num() - 1]->SetEventTriggerState(false);
            }
            tableCardNode4AGo2.Add(mCardItem);
        }
        else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            int nTop7Index = targetPosTypeInfo[1];
            auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTop7Index];
            mListCardNodeTop7Go.Add(mCardItem);
            this->DoTop7ReSizeHeightAni(nTop7Index);
        }
        else
        {
            ensure(false);
        }

        this->OnDragBegin(mCardItem);
        this->OnDragEndMove(mCardItem, fromPos, toPos, false);
    }
    else if (oriPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        int nOriTop7Index = oriPosTypeInfo[1];
        int nRemoveIndex = oriPosTypeInfo[2];
        auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nOriTop7Index];

        if (targetPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
        {
            bool bUseMagicWand = nRemoveIndex < mListCardNodeTop7Go.Num();
            auto mCardItem = TArrayExtentions::Remove(mListCardNodeTop7Go, nRemoveIndex);
            if (bUseMagicWand)
            {
                mCardItem->SetTurnOverState(true, nStepIndex - 1);
                mCardItem->PlayTurnOverAni();
                mCardItem->SetEventTriggerState(true);
            }
            else
            {
                auto nNowLastTop7CardItem = mListCardNodeTop7Go[mListCardNodeTop7Go.Num() - 1];
                if (nNowLastTop7CardItem && nNowLastTop7CardItem->orTurnOverStateIsTrue() == false)
                {
                    nNowLastTop7CardItem->SetTurnOverState(true, nStepIndex - 1);
                    nNowLastTop7CardItem->PlayTurnOverAni();
                    nNowLastTop7CardItem->SetEventTriggerState(true);
                }
            }

            int nA4Index = targetPosTypeInfo[1];
            auto& mListCardNode4AGo = this->tableCardNode4AGo[nA4Index];
            if (mListCardNode4AGo.Num() > 0)
            {
                mListCardNode4AGo[mListCardNode4AGo.Num() - 1]->SetEventTriggerState(false);
            }
            mListCardNode4AGo.Add(mCardItem);

            this->DoTop7ReSizeHeightAni(nOriTop7Index);
            this->OnDragBegin(mCardItem);
            this->OnDragEndMove(mCardItem, fromPos, toPos, false);
        }
        else if (targetPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
        {
            auto tableCardList = this->RemoveArrayFromTop7Go(nOriTop7Index, nRemoveIndex);
            auto mCardItem = tableCardList[0];
            auto nNowLastTop7CardItem = mListCardNodeTop7Go[mListCardNodeTop7Go.Num() - 1];
            if (nNowLastTop7CardItem and nNowLastTop7CardItem->orTurnOverStateIsTrue() == false)
            {
                nNowLastTop7CardItem->SetTurnOverState(true, nStepIndex - 1);
                nNowLastTop7CardItem->PlayTurnOverAni();
                nNowLastTop7CardItem->SetEventTriggerState(true);
            }

            int nTargetTop7Index = targetPosTypeInfo[1];
            this->InsertArrayToTop7Go(nTargetTop7Index, tableCardList);
            this->DoTop7ReSizeHeightAni(nOriTop7Index);
            this->DoTop7ReSizeHeightAni(nTargetTop7Index);

            auto& tableCardNodeTop7Go_To = this->tableCardNodeTop7Go[nTargetTop7Index];

            this->OnDragBegin(mCardItem);
            auto mCardItemList = tableCardList;
            for (int i = 0; i < mCardItemList.Num(); i++)
            {
                auto mTempCardItem = mCardItemList[i];
                auto from = this->GetCardNodeTop7Pos(nOriTop7Index, nRemoveIndex + i - 1);
                auto to = this->GetCardNodeTop7Pos(nTargetTop7Index, tableCardNodeTop7Go_To.Num() - mCardItemList.Num() + i);
                this->OnDragEndMove(mTempCardItem, from, to, false);
            }
        }
        else
        {
            ensure(false);
        }
    }

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
}