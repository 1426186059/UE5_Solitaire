// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"

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

    UButton* mGameNodeBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("gameNodeBtn"));
    mGameNodeBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_GameNodeBtn);

    this->PokerItemParent = Cast<UCanvasPanel>(mUIRoot->GetWidgetFromName("PokerItemParent"));

    this->tranFaPaiPos = UMGHelper::GetRelativePos(this->PokerItemParent, mUIRoot->GetWidgetFromName("FaPaiPos"));
    this->mCardNodeDraw3BeginPos = UMGHelper::GetRelativePos(this->PokerItemParent, mUIRoot->GetWidgetFromName("Draw3Pos1"));

    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget tranFaPaiPos: %s"), *this->tranFaPaiPos.ToString());
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget mCardNodeDraw3BeginPos: %s"), *this->mCardNodeDraw3BeginPos.ToString());
    
    this->tableCardNode4APos = {};
    for (int i = 1; i <= 4; i++)
    {
        FString Key = FString::Printf(TEXT("4APos%d"), i);
        FVector2D mPos = UMGHelper::GetRelativePos(this->PokerItemParent, mUIRoot->GetWidgetFromName(*Key));
        this->tableCardNode4APos.Add(mPos);
    }

    this->tableCardNodeTop7Pos = {};
    for (int i = 1; i <= 7; i++)
    {
        FString Key = FString::Printf(TEXT("Top7Pos%d"), i);
        FVector2D mPos = UMGHelper::GetRelativePos(this->PokerItemParent, mUIRoot->GetWidgetFromName(*Key));
        this->tableCardNodeTop7Pos.Add(mPos);

        UE_LOG(LogTemp, Log, TEXT("UMainUIWidget tableCardNodeTop7Pos: %s"), *mPos.ToString());
    }
    
    UButton* SendPokerBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("SendPokerBtn"));
    SendPokerBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_SendPokerBtn);
    this->mBG = Cast<UImage>(mUIRoot->GetWidgetFromName("BG"));
    UButton* BGBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("BGBtn"));
    BGBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_BGBtn);

    //this->TopBottomView:UpdateShowHideAni(false);  
    //if (LuaGameConfig.PLATFORM_EDITOR)
    //{
    //    TestView->Show();
    //}

    GetWorld()->GetTimerManager().SetTimer(
        this->mTimer, 
        this,
        &UMainUIWidget::TimerPerSecondUpdate,
        1.0f, 
        true
    );
    UEHelper::PauseTimer(this, this->mTimer);
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
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget OnBtnClicked_GameNodeBtn"));
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
    //this->TopBottomView->UpdateShowHideAni();
}

void UMainUIWidget::RecycleAndInitCardGo()
{
    //------------------------------回收资源------------------------------------
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
        {
            this->mSendCardListGo.Add(tableCardNodeTop7Go[i][j]);
        }
    }
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < this->tableCardNode4AGo[i].Num(); j++)
        {
            this->mSendCardListGo.Add(this->tableCardNode4AGo[i][j]);
        }
    }
    
    for (auto v : this->tableCardDraw3Go)
    {
        this->mSendCardListGo.Add(v);
    }
    
    //----------------------------------初始化资源------------------------------------
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

    for (int i = 0; i < 52; i++)
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
    auto tableOpStepItem = RecordStepDataHandler::GetSingleton()->data->tableOpStepItem;
    auto mInitSendCardList = RecordStepDataHandler::GetSingleton()->data->mInitSendCardList;
    if (RecordStepDataHandler::GetSingleton()->orGameEnd())
    {
        this->NewGameBegin_ForNormal(true);
        return;
    }
    this->mLastSendCardList = mInitSendCardList;

    //-------------------------- - 初始化资源------------------------------

    this->RecycleAndInitCardGo();
    this->bGameEnd = false;
    this->nGameMode = (SolitaireGameMode)RecordStepDataHandler::GetSingleton()->data->nGameMode;
    this->PokerItemParent->SetVisibility(ESlateVisibility::Collapsed);

    ensureMsgf(mInitSendCardList.Num() == 52, TEXT("mInitSendCardList Error: %d"), mInitSendCardList.Num());
    for (int i = 0; i < mInitSendCardList.Num(); i++)
    {
        this->mSendCardListGo[i]->SetPokerId(mInitSendCardList[i]);
        this->mSendCardListGo[i]->SetTurnOverState(false);
    }

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            int nTopIndex = i;
            UPokerItemWidget* mCardItem = this->mSendCardListGo.Pop();
            this->tableCardNodeTop7Go[nTopIndex].Add(mCardItem);
            UMGHelper::SetSlotPos(mCardItem, this->GetCardNodeTop7MaxHeightPos(nTopIndex));
            if (i == j)
            {
                mCardItem->SetTurnOverState(true);
            }
        }
    }

    if (bPlayAni)
    {
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNodeTop7Go[i][j];
                UMGHelper::SetAsLastChildIndex(mCardItem);
                mCardItem->Show();
                mCardItem->ForceShowBackUI();
                mCardItem->SetEventTriggerState(mCardItem->orTurnOverStateIsTrue());
                FVector2D fromPos = this->GetCardNodeSendPokerPos();
                UMGHelper::SetSlotPos(mCardItem, fromPos);
            }
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < this->tableCardNode4AGo[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNode4AGo[i][j];
                UMGHelper::SetAsLastChildIndex(mCardItem);
                mCardItem->ForceShowBackUI();
                mCardItem->Show();
                mCardItem->SetEventTriggerState(j + 1 == this->tableCardNode4AGo[i].Num());
                FVector2D fromPos = this->GetCardNodeSendPokerPos();
                UMGHelper::SetSlotPos(mCardItem, fromPos);
            }
        }

        for (int i = 0; i < this->tableCardDraw3Go.Num(); i++)
        {
            auto mCardItem = this->tableCardDraw3Go[i];
            UMGHelper::SetAsLastChildIndex(mCardItem);
            mCardItem->ForceShowBackUI();
            mCardItem->SetEventTriggerState(i == 1);

            if (i <= 3)
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

        // ------------------ - 动画播放-------------------- -
        AudioHandler::GetSingleton()->PlaySound(TEXT("start_new"));
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNodeTop7Go[i][j];
                auto fromPos = this->GetCardNodeSendPokerPos();
                auto toPos = this->GetCardNodeTop7Pos(i, j);

                /*local mTween = TweenMgr:AddGoTween(this->transform, 0.3, function(fTimePercent)
                    this->SetRelativePos(mCardItem, TweenCommonFunc:easeOutQuad(fromPos, toPos, fTimePercent))
                    end, function()
                    if mCardItem : orTurnOverStateIsTrue() then
                        mCardItem : PlayTurnOverAni()
                        end
                        end) : SetDelay(0.05 * j)
                        end
                        end

                        TweenMgr : delayedCall(0.5, function()
                            local n4ACount = 0
                            for i = 1, 4 do
                                for j = 1, #this->tableCardNode4AGo[i] do
                                    n4ACount = n4ACount + 1

                                    local mCardItem = this->tableCardNode4AGo[i][j]
                                    local fromPos = this->GetCardNodeSendPokerPos()
                                    local toPos = this-> GetCardNode4APos(i)
                                    local mTween = TweenMgr : AddGoTween(this->transform, 0.3, function(fTimePercent)
                                        this->SetRelativePos(mCardItem, TweenCommonFunc:easeOutQuad(fromPos, toPos, fTimePercent))
                                        end, function()
                                        if mCardItem : orTurnOverStateIsTrue() then
                                            mCardItem : PlayTurnOverAni()
                                            end
                                            end)
                                    end
                                    end

                                    if n4ACount > 0 then
                                        AudioHandler : PlaySound("P2")
                                        end
                                        end)

                        TweenMgr : delayedCall(1.0, function()
                            local nDraw3Count = 0
                            for i = 1, #this->tableCardDraw3Go do
                                nDraw3Count = nDraw3Count + 1

                                local mCardItem = this->tableCardDraw3Go[i]
                                local fromPos = this->GetCardNodeSendPokerPos()
                                local toPos = this-> GetCardNodeDraw3Pos(i)
                                local mTween = TweenMgr : AddGoTween(this->transform, 0.3, function(fTimePercent)
                                    this->SetRelativePos(mCardItem, TweenCommonFunc:easeOutQuad(fromPos, toPos, fTimePercent))
                                    end, function()
                                    if mCardItem : orTurnOverStateIsTrue() then
                                        mCardItem : PlayTurnOverAni()
                                        end
                                        end)
                                end

                                if nDraw3Count > 0 then
                                    AudioHandler : PlaySound("P2")
                                    end
                                    end)
                                else
                                    this->RefreshAllPokerState()
                                    end

                                    GameEventHandler : Brocast(EventName.RefreshTopBottomUI)
                                    this->mTimer : Start()
                                    this-> UpdateGameMode()
                                    this-> ResetRemainHintCount()
                                    this-> onAddScore_InitParam()
                                    this->GameWinAniMgr : DestroyAniNode()
                                    this-> DoWhenSet_FastGame()*/
                                    //}
            }
        }
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
        if (RecordStepDataHandler::GetSingleton()->data->nGameMode == this->nGameMode &&
            RecordStepDataHandler::GetSingleton()->orGameEnd() == false)
        {
            //--牌局未改变
        }
        else
        {
            auto nLastRecordData = AllRecordDataHandler::GetSingleton()->RemoveAndGetLastDifferentGameModeRecordTable(this->nGameMode);
            if (nLastRecordData)
            {
                RecordStepDataHandler::GetSingleton()->InitStepRecordFromOther(nLastRecordData);
                this->RecoverGame();
            }
            else
            {
                this->NewGameBegin(false);
            }
        }
    }
}

void UMainUIWidget::NewGameBegin(bool bRePlay)
{
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget NewGameBegin"));
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
    if (DataCenter::GetSingleton()->data->nTotalGameCount > 0)
    {
        if (RecordStepDataHandler::GetSingleton()->data->nGameMode == SolitaireGameMode::Normal)
        {
            if (RecordStepDataHandler::GetSingleton()->data->bWin)
            {
                DataCenter::GetSingleton()->AddNomalModeTotalWinCount();
            }
            DataCenter::GetSingleton()->AddDifficultLayerWinResult(RecordStepDataHandler::GetSingleton()->data->bWin);
        }

        if (RecordStepDataHandler::GetSingleton()->data->bWin)
        {
            DataCenter::GetSingleton()->AddTotalWinGameCount();
        }

        StatisticDataHandler::GetSingleton()->onGameStatistic(
            RecordStepDataHandler::GetSingleton()->data->bWin,
            RecordStepDataHandler::GetSingleton()->data->nMoveCount,
            RecordStepDataHandler::GetSingleton()->data->nTime,
            RecordStepDataHandler::GetSingleton()->data->nScore);

        if (RecordStepDataHandler::GetSingleton()->data->bWin)
        {
            if (DataCenter::GetSingleton()->data->nNomalModeTotalWinCount == 2 && DataCenter::GetSingleton()->data->bFastGame == false)
            {
                KKTween::delayedCall(1.0, []()
                    {
                        //ThemeSolitaire.Guide_FastPlayView:Show()
                    });
            };

            if (DataCenter::GetSingleton()->data->nNomalModeTotalWinCount == 4 and DataCenter::GetSingleton()->data->nMusicIndex == 0)
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

    ensureMsgf(mSendCardList.Num() == 52, TEXT("%d"), mSendCardList.Num());
    ensureMsgf(this->mSendCardListGo.Num() == 52, TEXT("%d"), this->mSendCardListGo.Num());
    RecordStepDataHandler::GetSingleton()->InitStepRecord(this->nGameMode, mSendCardList);
    //CollectPokerTaskDataHandler::GetSingleton()->SetGameBeginPokerId(mSendCardList);

    for (int i = 0; i < mSendCardList.Num(); i++)
    {
        int32 nPokerId = mSendCardList[i];
        this->mSendCardListGo[i]->SetPokerId(nPokerId);
        this->mSendCardListGo[i]->Show();
        this->mSendCardListGo[i]->SetTurnOverState(false);
        this->mSendCardListGo[i]->Refresh();
        this->mSendCardListGo[i]->SetEventTriggerState(false);
        UMGHelper::SetZOrder(this->mSendCardListGo[i], i);
        UMGHelper::SetSlotPos(this->mSendCardListGo[i], this->tranFaPaiPos);
    }

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
    //--------------------------- 做发牌动画---------------------------------
    AudioHandler::GetSingleton()->PlaySound(TEXT("start_new"));

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            int32 nTopIndex = i;
            auto mCardItem = this->mSendCardListGo.Pop();

            int32 nHeightIndex = this->tableCardNodeTop7Go[nTopIndex].Num();
            bool bTurnOverState = nHeightIndex == nTopIndex;
            if (bTurnOverState)
            {
                mCardItem->SetTurnOverState(true);
            }

            UMGHelper::SetZOrder(mCardItem, j);
            FVector2D from = UMGHelper::GetSlotPos(mCardItem);
            FVector2D to = this->GetCardNodeTop7MaxHeightPos(nTopIndex);
            //UMGHelper::SetSlotPos(mCardItem, to);
            this->tableCardNodeTop7Go[nTopIndex].Add(mCardItem);

            //UMGHelper::SetRenderPos(mCardItem, this->tranFaPaiPos - to);
            //to = FVector2D::ZeroVector;

            KKTween::UMG_MoveLocal_Slot(mCardItem, to, 0.3)->SetDelay(0.05 * j)->SetOnCompleteFunc([=, this]()
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

    this->GameWinAniMgr.DestroyAniNode();
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
        int nAniIndex = 0;
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
        mOpStepItemData->fromPosTypeInfo = { SolitairePokerPosType::Draw3Pos, 0, 0 };
        mOpStepItemData->toPosTypeInfo = { SolitairePokerPosType::SendPokerPos, 0, 0 };
        RecordStepDataHandler::GetSingleton()->AddStepRecord(mOpStepItemData);

        AudioHandler::GetSingleton()->PlaySound("P4");
        KKTween::delayedCall(this, 0.5, [this]()
            {
                this->DoWhenSet_FastGame();
            });
    }
    else
    {
        int nMaxDrawCount = FMath::Min(data->nDrawCount, this->mSendCardListGo.Num());
        for (int i = 0; i < nMaxDrawCount; i++)
        {
            auto mCardItem = TArrayExtentions::Remove(this->mSendCardListGo);
            this->tableCardDraw3Go.Insert(mCardItem, 0);
            UMGHelper::SetZOrder(mCardItem, 100 + i);
            mCardItem->SetTurnOverState(true);
            mCardItem->PlayTurnOverAni();
        }

        auto mOpStepItemData = RecordStepDataHandler::GetSingleton()->GetOpStepItemDefaultData();
        mOpStepItemData->fromPosTypeInfo = { SolitairePokerPosType::SendPokerPos, nMaxDrawCount, 0 };
        mOpStepItemData->toPosTypeInfo = { SolitairePokerPosType::Draw3Pos, 0, 0 };
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
    int nMaxCount = FMath::Min(3, this->tableCardDraw3Go.Num());
    for (int i = 0; i < nMaxCount; i++)
    {
        UPokerItemWidget* mCardItem = this->tableCardDraw3Go[i];
        mCardItem->Show();

        FVector2D from = UMGHelper::GetSlotPos(mCardItem);
        FVector2D to = this->GetCardNodeDraw3Pos(i);
        mCardItem->SetEventTriggerState(i == 0);
        this->OnClickDraw3Move(mCardItem, from, to, false);
    }

    for (int i = 3; i < this->tableCardDraw3Go.Num(); i++)
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
    auto& posTypeInfo = this->GetPokerPosType(mCardItem);
    if (posTypeInfo[1] == SolitairePokerPosType::Draw3Pos)
    {
        for (int i = 0; i < 4; i++)
        {
            if (this->orCanIn4A(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::A4Pos, i);
                return;
            }
        }
        for (int i = 0; i < 7; i++)
        {
            if (this->orCanInNode7(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::Top7Pos, i);
                return;
            }
        }
    }
    else if (posTypeInfo[1] == SolitairePokerPosType::A4Pos)
    {
        for (int i = 0; i < 7; i++)
        {
            if (this->orCanInNode7(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::Top7Pos, i);
                return;
            }
        }
    }
    else if (posTypeInfo[1] == SolitairePokerPosType::Top7Pos)
    {
        for (int i = 0; i < 4; i++)
        {
            if (this->orCanIn4A(i, mCardItem))
            {
                this->OnDragBegin(mCardItem);
                this->LockTargetToMove(mCardItem, SolitairePokerPosType::A4Pos, i);
                return;
            }
        }

        for (int i = 0; i < 7; i++)
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
    auto& mCardItemList = this->GetSelectCardItemList(mDragCardItem);
    for (auto v : mCardItemList)
    {
        auto mCardItem = v;
        if (this->mapCardItemTween.Contain(mCardItem))
        {
            this->mapCardItemTween[mCardItem].Cancel();
            this->mapCardItemTween.Remove(mCardItem);
        }
        //v.transform : SetAsLastSibling();
    }
}

void UMainUIWidget::OnDrag(UPokerItemWidget* mDragCardItem)
{
    auto& mCardItemList = this->GetSelectCardItemList(mDragCardItem);
    auto& oriPosTypeInfo = this->GetPokerPosType(mDragCardItem);
    if (oriPosTypeInfo[1] == SolitairePokerPosType::Top7Pos)
    {
        int32 nTop7Index = oriPosTypeInfo[2];
        int32 nIndex = oriPosTypeInfo[3];
        FVector2D oriPos = UMGHelper::GetSlotPos(mDragCardItem);
        for (int i = 0; i < mCardItemList.Num(); i++)
        {
            auto& mCardItem = mCardItemList[i];
            if (i > 0)
            {
                FVector2D Pos = oriPos - FVector2D(0, i * N_TOP7_GAP_HEIGHT);
                UMGHelper::SetSlotPos(mCardItem, Pos);
            }
        }
    }
}

void UMainUIWidget::OnDragEndToMovePokerPos(UPokerItemWidget* mCardItem)
{
    auto& mCardItemPos = this->GetRelativePosByGo(mCardItem);
    float mCardItemPosY = mCardItemPos.y;
    float orInZone = [=](FVector2D middlePos)
        {
            if (mCardItemPos.x >= middlePos.x - 80 && mCardItemPos.x <= middlePos.x + 80 &&
                mCardItemPos.y >= middlePos.y - 80 && mCardItemPos.y <= middlePos.y + 80)
            {
                return true;
            }

            return false;
        };

    bool bIn4APos = mCardItemPosY > this->tableCardNode4APos[1].y - 75;
    if (bIn4APos)
    {
        int n4AIndex = -1;
        for (int i = 0; i < 4; i++)
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
        int nTop7Index = -1;
        for (int i = 0; i < 7; i++)
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

    this->LockTargetToMove(mCardItem, -1);
}

void UMainUIWidget::LockTargetToMove(UPokerItemWidget* mCardItem, int nPosType, int nIndex)
{
    auto& oriPosTypeInfo = this->GetPokerPosType(mCardItem);
    auto& mCardItemList = this->GetSelectCardItemList(mCardItem);

    bool bToOtherPos = false;
    if (nPosType == SolitairePokerPosType::A4Pos)
    {
        int n4AIndex = nIndex;
        auto& tableCardNode4AGo = this->tableCardNode4AGo[n4AIndex];
        if (tableCardNode4AGo.Num() > 0)
        {
            tableCardNode4AGo[tableCardNode4AGo.Num() - 1]->SetEventTriggerState(false);
        }

        this->SetDragEndRemoveSelfFromArray(mCardItem);
        tableCardNode4AGo.Add(mCardItem);
        bToOtherPos = true;

        if (tableCardNode4AGo.Num() < 8)
        {
        AudioHandler: PlaySound("receive_card_"..#tableCardNode4AGo);
        }
        else
        {
        AudioHandler:PlaySound("receive_card_8");
        }
    }
    else if (nPosType == SolitairePokerPosType::Top7Pos)
    {
        int nTop7Index = nIndex;
        auto& tableCardNodeTop7Go = this->tableCardNodeTop7Go[nTop7Index];
        int nNowHeight = #tableCardNodeTop7Go;
        bToOtherPos = true;

        local tableArray = this->SetDragEndRemoveSelfFromArray(mCardItem);
        this->InsertArrayToTop7Go(nTop7Index, tableArray);
        AudioHandler: PlaySound("P2");
    }

    auto targetPosTypeInfo;
    if (bToOtherPos)
    {
        ThemeSolitaire.RecordStepDataHandler : AddMoveCount();
        targetPosTypeInfo = this->GetPokerPosType(mCardItem);

        auto mOpStepItemData = RecordStepDataHandler::GetSingleton()->GetOpStepItemDefaultData();
        mOpStepItemData.fromPosTypeInfo = oriPosTypeInfo;
        mOpStepItemData.toPosTypeInfo = targetPosTypeInfo;
        RecordStepDataHandler: AddStepRecord(mOpStepItemData);

        this->onAddScore();
        this->onAddIQ();
        this->DoWhenSet_FastGame();

        if (oriPosTypeInfo[1] == SolitairePokerPosType::Top7Pos)
        {
            this->DoTop7ReSizeHeightAni(oriPosTypeInfo[2]);
        }

        if (targetPosTypeInfo[1] == SolitairePokerPosType::Top7Pos)
        {
            this->DoTop7ReSizeHeightAni(targetPosTypeInfo[2]);
        }
    }

    for (auto v : mCardItemList)
    {
        auto mTempCardItem = v;
        auto cardPosTypeInfo = this->GetPokerPosType(v);
        FVector2D from = this->GetRelativePosByGo(v);
        FVector2D to = this->GetPosByPosTypeInfo(cardPosTypeInfo);

        this->OnDragEndMove(mTempCardItem, from, to, false, [=]()
            {
                if (targetPosTypeInfo)
                {
                    if (targetPosTypeInfo[1] == SolitairePokerPosType::A4Pos)
                    {
                        this->PlayToA4EffectAni(targetPosTypeInfo[2], mTempCardItem);
                    }
                }
            });
    }

    if ((not this->bGameEnd) && oriPosTypeInfo[1] != SolitairePokerPosType::A4Pos)
    {
        if (this->tween_OnFastGameToResultA4)
        {
            this->tween_OnFastGameToResultA4:cancel();
            this->tween_OnFastGameToResultA4 = nil;
        }

        this->tween_OnFastGameToResultA4 = KKTween::delayedCallWithGo(this->transform, 0.35, []()
            {
                this->OnFastGameToResultA4();
            });
    }

    this->DoActionEnd();
}



//------------------------------------------------------游戏模式相关--------------------------------------------------------------------
void UMainUIWidget::UpdateGameMode()
{
    UE_LOG(LogTemp, Log, TEXT("this->nGameMode: %d"), this->nGameMode);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateGameModeState)->Broadcast(nullptr);
    if (this->nGameMode == SolitaireGameMode::Trip)
    {
        KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdateTripState)->Broadcast(nullptr);
    }
}

//----------------------------------------------------------- 增加分数--------------------------------------------------------------------------
std::tuple<bool, UPokerItemWidget*> UMainUIWidget::orThisStepTurnOverPokerIsTrue(int nStepIndex)
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
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

int UMainUIWidget::GetTop7HideCardCount()
{
    int nCount = 0;
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
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
    int baseScore = 0;
    int addScore = 0;

    auto& tableOpStepItem = RecordStepDataHandler::GetSingleton()->data->tableOpStepItem;
    auto& nLastOpInfo = tableOpStepItem[tableOpStepItem.Num() - 1];
    auto& fromPosTypeInfo = nLastOpInfo->fromPosTypeInfo;
    auto& toPosTypeInfo = nLastOpInfo->toPosTypeInfo;

    int nAddSumScore = 0;
    if (toPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
    {
        nAddSumScore = nAddSumScore + 5;
        int nLastIndex = tableOpStepItem.Num() - 1;
        int nContinueToA4StepCount = 0;
        while (nLastIndex >= 0)
        {
            auto& nTempLastOpInfo = tableOpStepItem[nLastIndex];
            nLastIndex = nLastIndex - 1;
            if (nTempLastOpInfo->toPosTypeInfo[0] == SolitairePokerPosType::A4Pos)
            {
                nContinueToA4StepCount = nContinueToA4StepCount + 1;
            }
            else
            {
                break;
            }
        }

        nAddSumScore = nAddSumScore + 5 * nContinueToA4StepCount;
    }
    else if (toPosTypeInfo[0] == SolitairePokerPosType::Top7Pos)
    {
        if (fromPosTypeInfo[0] == SolitairePokerPosType::Draw3Pos)
        {
            nAddSumScore = nAddSumScore + 5;
        }

        int nLastIndex = tableOpStepItem.Num() - 1;
        int nContinueCount = 0;
        while (nLastIndex >= 0)
        {
            auto nTempLastOpInfo = tableOpStepItem[nLastIndex];
            nLastIndex = nLastIndex - 1;
            if (nTempLastOpInfo->toPosTypeInfo[0] == SolitairePokerPosType::Top7Pos and nTempLastOpInfo->nTureOverPokerId == nLastOpInfo->nTureOverPokerId)
            {
                nContinueCount = nContinueCount + 1;
            }
            else
            {
                break;
            }
        }

        nAddSumScore = nAddSumScore + 5 * nContinueCount;
    }

    int nAddNewPokerCount = this->nGetScore_nLastTop7HideCardCount - this->GetTop7HideCardCount();
    if (nAddNewPokerCount > 0)
    {
        this->nGetScore_nLastTop7HideCardCount = this->GetTop7HideCardCount();
        nAddSumScore = nAddSumScore + nAddNewPokerCount * 5;
    }

    nLastOpInfo->nScore = nAddSumScore;
    RecordStepDataHandler::GetSingleton()->AddScore(nAddSumScore);
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
}

//------------------------------------ 相对位置计算 --------------------------------------------
float UMainUIWidget::GetTop7_Gap_Height(int nTopIndex)
{
    auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    int nStartPosIndex = 5;
    int nZhengCount = 0;
    for(auto& v : mListCardNodeTop7Go)
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

FVector2D UMainUIWidget::GetCardNodeTop7MaxHeightPos(int nTopIndex)
{
    return this->GetCardNodeTop7Pos(nTopIndex, this->tableCardNodeTop7Go[nTopIndex].Num());
}

FVector2D UMainUIWidget::GetCardNodeTop7NextMaxHeightPos(int nTopIndex)
{
    return this->GetCardNodeTop7Pos(nTopIndex, this->tableCardNodeTop7Go[nTopIndex].Num() + 1);
}

FVector2D UMainUIWidget::GetCardNodeTop7Pos(int nTopIndex, int nNowCount)
{
    float nGapZhengDis = this->GetTop7_Gap_Height(nTopIndex);
    FVector2D oriPos = this->tableCardNodeTop7Pos[nTopIndex];
    auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    float posY = oriPos.Y;
    for (int i = 0; i < nNowCount; i++)
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

FVector2D UMainUIWidget::GetCardNode4APos(int nTopIndex)
{
    return this->tableCardNode4APos[nTopIndex];
}

FVector2D UMainUIWidget::GetCardNodeDraw3Pos(int nIndex)
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

void UMainUIWidget::DoTop7ReSizeHeightAni(int nTop7Index)
{
    auto& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTop7Index];
    for (int i = 0; i < mListCardNodeTop7Go.Num(); i++)
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
        this->mapCardItemTween[mCardItem] = mTween;
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
    for (int i = 0; i < popCanvas->GetChildrenCount(); i++)
    {
        if (popCanvas->GetChildAt(i)->IsVisibleInDesigner())
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

void UMainUIWidget::Set_AutoHinted()
{
    if (DataCenter::GetSingleton()->data->bAutoHint)
    {
       //this->PlayHintAni();
    }
}

//---------------------------------------------------------- - 设置里的通用方法--------------------------------------------------------------------------
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