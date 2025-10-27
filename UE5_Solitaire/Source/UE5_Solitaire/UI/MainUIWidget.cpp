// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"

void UMainUIWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget NativeOnInitialized"));
}

void UMainUIWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget NativeConstruct"));
}

void UMainUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FVector2D mSize = UMGHelper::GetUMGRootSzie(this->mUIRoot);
    if (mSize != mOldSize)
    {
        mOldSize = mSize;
        this->OnScreenSizeChanged();
    }
}

void UMainUIWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget Init"));

    mUIRoot = Cast<UUserWidget>(GetWidgetFromName(TEXT("MainUIWBP")));
    if (!mUIRoot)
    {
        UE_LOG(LogTemp, Error, TEXT("mUIRoot == null"));
        return;
    }

    this->AddToViewport(0);

    UButton* mGameNodeBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("gameNodeBtn"));
    mGameNodeBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_GameNodeBtn);

    this->tranFaPaiPos = mUIRoot->GetWidgetFromName("FaPaiPos")->GetRenderTransform().Translation;
    this->mCardNodeDraw3BeginPos = mUIRoot->GetWidgetFromName("Draw3Pos1")->GetRenderTransform().Translation;
    
    this->tableCardNode4APos = {};
    for (int i = 1; i <= 4; i++)
    {
        FName Key = FName(FString::Printf(TEXT("4APos%d"), i));
        FVector2D mPos = mUIRoot->GetWidgetFromName(Key)->GetRenderTransform().Translation;
        this->tableCardNode4APos.Add(mPos);
    }
    this->tableCardNodeTop7Pos = {};
    for (int i = 1; i <= 7; i++)
    {
        FName Key = FName(FString::Printf(TEXT("Top7Pos%d"), i));
        FVector2D mPos = mUIRoot->GetWidgetFromName(Key)->GetRenderTransform().Translation;
        this->tableCardNodeTop7Pos.Add(mPos);
    }

    this->PokerItemParent = Cast<UCanvasPanel>(mUIRoot->GetWidgetFromName("PokerItemParent"));

}

void UMainUIWidget::Show()
{
    this->Init();
    this->SetVisibility(ESlateVisibility::Visible);
    this->InitGame();
}

void UMainUIWidget::Hide()
{
    this->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainUIWidget::Refresh()
{
    //mUProgressBar->SetPercent(0);
}

void UMainUIWidget::OnScreenSizeChanged()
{
    //BG 适配屏幕
    auto mBG = Cast<UImage>(mUIRoot->GetWidgetFromName(TEXT("BG")));
    UMGAdapterTool::GetSingleton()->FitBG(mUIRoot, mBG);
}

void UMainUIWidget::OnBtnClicked_GameNodeBtn()
{
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget OnBtnClicked_GameNodeBtn"));
}



void UMainUIWidget::RecycleAndInitCardGo()
{

}

void UMainUIWidget::InitGame()
{
    //初始化播放发牌动画
    mSendCardListGo = {};
    TSubclassOf<UPokerItemWidget> PokerItemWBP = LoadClass<UPokerItemWidget>(nullptr,
        TEXT("/Game/ResourceABs/MainScene/BPS/PokerItemCWBP.PokerItemCWBP_C"));

    if (PokerItemWBP == NULL)
    {
        UE_LOG(LogTemp, Log, TEXT("UMainUIWidget OnBtnClicked_GameNodeBtn"));
        return;
    }

    for (int i = 0; i < 52; i++)
    {
        UPokerItemWidget* mPokerItem = CreateWidget<UPokerItemWidget>(GEngine->GameViewport->GetWorld(), PokerItemWBP);
        this->PokerItemParent->AddChild(mPokerItem);
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
    //this->PokerItemParent->SetVisibility(ESlateVisibility::Collapsed);

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
            this->SetRelativePos(mCardItem, this->GetCardNodeTop7MaxHeightPos(nTopIndex));
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
                UMGHelper::SetPosition(mCardItem, fromPos);
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
                UMGHelper::SetPosition(mCardItem, fromPos);
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
            UMGHelper::SetPosition(mCardItem, fromPos);
        }

        // ------------------ - 动画播放-------------------- -
        UAudioHandler::GetSingleton()->PlaySound("start_new");
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < this->tableCardNodeTop7Go[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNodeTop7Go[i][j];
                auto fromPos = this->GetCardNodeSendPokerPos();
                auto toPos = this->GetCardNodeTop7Pos(i, j);

                /*local mTween = TweenMgr:AddGoTween(self.transform, 0.3, function(fTimePercent)
                    self:SetRelativePos(mCardItem, TweenCommonFunc:easeOutQuad(fromPos, toPos, fTimePercent))
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
                                for j = 1, #self.tableCardNode4AGo[i] do
                                    n4ACount = n4ACount + 1

                                    local mCardItem = self.tableCardNode4AGo[i][j]
                                    local fromPos = self:GetCardNodeSendPokerPos()
                                    local toPos = self : GetCardNode4APos(i)
                                    local mTween = TweenMgr : AddGoTween(self.transform, 0.3, function(fTimePercent)
                                        self:SetRelativePos(mCardItem, TweenCommonFunc:easeOutQuad(fromPos, toPos, fTimePercent))
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
                            for i = 1, #self.tableCardDraw3Go do
                                nDraw3Count = nDraw3Count + 1

                                local mCardItem = self.tableCardDraw3Go[i]
                                local fromPos = self:GetCardNodeSendPokerPos()
                                local toPos = self : GetCardNodeDraw3Pos(i)
                                local mTween = TweenMgr : AddGoTween(self.transform, 0.3, function(fTimePercent)
                                    self:SetRelativePos(mCardItem, TweenCommonFunc:easeOutQuad(fromPos, toPos, fTimePercent))
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
                                    self:RefreshAllPokerState()
                                    end

                                    GameEventHandler : Brocast(EventName.RefreshTopBottomUI)
                                    self.mTimer : Start()
                                    self : UpdateGameMode()
                                    self : ResetRemainHintCount()
                                    self : onAddScore_InitParam()
                                    self.GameWinAniMgr : DestroyAniNode()
                                    self : DoWhenSet_FastGame()*/
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
   // ----------------------------------------------广告--------------------------------------------
   /* if ThemeSolitaire.Config : orTriggerBannerAds() then
        if not GoogleAdsHandler.bShowBannerAds then
            GoogleAdsHandler : Show_BannerAds(true)
            end
            end

            if ThemeSolitaire.Config : orTriggerRewardedInterstitialAds() then
                GoogleAdsHandler : Show_RewardedInterstitialAds()
                end

                --------------------------------------对上把数据 记录整理--------------------------------------------
                if DataCenter.data.nTotalGameCount > 0 then
                    if RecordStepDataHandler.data.nGameMode == ThemeSolitaire.GameMode.Normal then
                        if RecordStepDataHandler.data.bWin then
                            DataCenter : AddNomalModeTotalWinCount()
                            end
                            DataCenter : AddDifficultLayerWinResult(RecordStepDataHandler.data.bWin)
                            end

                            if RecordStepDataHandler.data.bWin then
                                DataCenter : AddTotalWinGameCount()
                                end

                                StatisticDataHandler : onGameStatistic(
                                    RecordStepDataHandler.data.bWin,
                                    RecordStepDataHandler.data.nMoveCount,
                                    RecordStepDataHandler.data.nTime,
                                    RecordStepDataHandler.data.nScore)

                                if RecordStepDataHandler.data.bWin then
                                    if DataCenter.data.nNomalModeTotalWinCount == 2 and DataCenter.data.bFastGame == false then
                                        TweenMgr : delayedCall(1.0, function()
                                            ThemeSolitaire.Guide_FastPlayView:Show()
                                            end)
                                        end

                                        if DataCenter.data.nNomalModeTotalWinCount == 4 and DataCenter.data.nMusicIndex == 0 then
                                            TweenMgr : delayedCall(1.0, function()
                                                ThemeSolitaire.Guide_MusicOnView:Show()
                                                end)
                                            end
                                            end
                                            end

                                            ----------------------------初始化数据--------------------------------------------
                                            DataCenter.data.nDifficultLayer = LuaHelper.Clamp(DataCenter.data.nDifficultLayer, 1, 10)
                                            if not bRePlay then
                                                DataCenter : AddGameLevel()
                                                end

                                                DataCenter.data.nIQValue = 100
                                                DataCenter : AddTotalGameCount()
                                                self : TellRobot_PlayerAlive()
                                                self : RecycleAndInitCardGo()
                                                self.bGameEnd = false
                                                self.PokerItemParent.gameObject : SetActive(true)

                                                --------------------------------------事件---------------------------------- -
                                                self : PrintGameStartInfo()

                                                local mSendCardList = nil
                                                if bRePlay and self.mLastSendCardList then
                                                    mSendCardList = self.mLastSendCardList
                                                else
                                                    if self.nGameMode == ThemeSolitaire.GameMode.DailyChallenge then
                                                        mSendCardList = CardHandler:GetInitCards_ForChallengeMode()
                                                        elseif self.nGameMode == ThemeSolitaire.GameMode.Rank then
                                                        mSendCardList = CardHandler : GetInitCards_ForRankMode()
                                                        elseif self.nGameMode == ThemeSolitaire.GameMode.Trip then
                                                        mSendCardList = CardHandler : GetInitCards_ForTripMode()
                                                    else
                                                        mSendCardList = CardHandler:GetInitCards_ForNormalMode()
                                                        end
                                                        end
                                                        self.mLastSendCardList = mSendCardList

                                                        Debug.Assert(#mSendCardList == 52, #mSendCardList)
                                                        Debug.Assert(#self.mSendCardListGo == 52, #self.mSendCardListGo)
                                                        RecordStepDataHandler:InitStepRecord(self.nGameMode, LuaHelper.DeepCloneTable(mSendCardList))
                                                        ThemeSolitaire.CollectPokerTaskDataHandler : SetGameBeginPokerId(mSendCardList)

                                                        for i = 1, #mSendCardList do
                                                            local nPokerId = mSendCardList[i]
                                                            self:SetRelativePos(self.mSendCardListGo[i], self.tranFaPaiPos)

                                                            self.mSendCardListGo[i] : SetPokerId(nPokerId)
                                                            self.mSendCardListGo[i] : Show()
                                                            self.mSendCardListGo[i] : SetTurnOverState(false)
                                                            self.mSendCardListGo[i] : RefreshUI()
                                                            self.mSendCardListGo[i] : SetEventTriggerState(false)
                                                            end

                                                            GameEventHandler : Brocast(EventName.RefreshTopBottomUI)
                                                            -------------------------- - 做发牌动画-------------------------------- -
                                                            AudioHandler : PlaySound("start_new")
                                                            for i = 1, 7 do
                                                                for j = 1, i do
                                                                    local nTopIndex = i
                                                                    local mCardItem = table.remove(self.mSendCardListGo)
                                                                    table.insert(self.tableCardNodeTop7Go[nTopIndex], mCardItem)
                                                                    local nHeightIndex = #self.tableCardNodeTop7Go[nTopIndex]
                                                                    local bTurnOverState = nHeightIndex == nTopIndex
                                                                    if bTurnOverState then
                                                                        mCardItem : SetTurnOverState(true)
                                                                        end
                                                                        mCardItem.transform : SetAsLastSibling()

                                                                        local from = self : GetRelativePosByGo(mCardItem)
                                                                        local to = self : GetCardNodeTop7MaxHeightPos(nTopIndex)
                                                                        self : SetRelativePos(mCardItem, from)
                                                                        LeanTween.moveLocal(mCardItem.transform.gameObject, to, 0.3) : setDelay(0.05 * (j - 1)) : setOnComplete(function()
                                                                            GameEventHandler:Brocast(EventName.RefreshTopBottomUI)
                                                                            if bTurnOverState then
                                                                                mCardItem : PlayTurnOverAni()
                                                                                mCardItem : SetEventTriggerState(true)
                                                                                end
                                                                                end)

                                                                        end
                                                                        end

                                                                        self.GameWinAniMgr:DestroyAniNode()
                                                                        self : UpdateGameMode()
                                                                        self : onAddScore_InitParam()
                                                                        self.mTimer : Start()
                                                                        self : TellRobot_PlayerAlive()
                                                                        self : ResetRemainHintCount()
                                                                        LeanTween.delayedCall(self.transform.gameObject, 1.0, function()
                                                                            self:DoWhenSet_FastGame()

                                                                            if ThemeSolitaire.Config.bRobotTest then
                                                                                self : StartRobotPlay()
                                                                                end
                                                                                end)*/
}

//------------------------------------ 相对位置计算 --------------------------------------------
FVector2D UMainUIWidget::GetRelativePosByGo(UWidget* target)
{
    return target->GetRenderTransform().Translation;
}

void UMainUIWidget::SetRelativePos(UWidget* target, FVector2D relativePos)
{
    target->SetRenderTranslation(relativePos);
}

float UMainUIWidget::GetTop7_Gap_Height(int nTopIndex)
{
    TArray<UPokerItemWidget*>& mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    int nStartPosIndex = 5;
    int nZhengCount = 0;
    for(auto v : mListCardNodeTop7Go)
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

FVector2D UMainUIWidget::GetCardNodeTop7Pos(int nTopIndex, int nHeightIndex)
{
    float nGapZhengDis = this->GetTop7_Gap_Height(nTopIndex);
    FVector2D oriPos = this->tableCardNodeTop7Pos[nTopIndex];
    TArray<UPokerItemWidget*> mListCardNodeTop7Go = this->tableCardNodeTop7Go[nTopIndex];
    float posY = oriPos.Y;
    for (int i = 1; i < nHeightIndex - 1; i++)
    {
        UPokerItemWidget* mCardItem = mListCardNodeTop7Go[i];
        if (mCardItem == nullptr or mCardItem->orTurnOverStateIsTrue())
        {
            posY = posY - nGapZhengDis;
        }
        else
        {
            posY = posY - 20;
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
    if (nIndex <= 3)
    {
        float fOffsetX = -44 * (nIndex - 1);
        return this->mCardNodeDraw3BeginPos + FVector2D(fOffsetX, 0);
    }
    else
    {
        float fOffsetX = -1 * (nIndex - 3);
        return this->GetCardNodeDraw3Pos(3) + FVector2D(fOffsetX, 0);
    }
}

FVector2D UMainUIWidget::GetCardNodeSendPokerPos()
{
    return this->tranFaPaiPos;
}