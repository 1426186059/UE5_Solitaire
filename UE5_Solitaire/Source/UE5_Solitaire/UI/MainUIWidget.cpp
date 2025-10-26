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
        FVector2D mPos = mUIRoot->GetWidgetFromName("4APos" + i)->GetRenderTransform().Translation;
        this->tableCardNode4APos.Add(mPos);
    }
    this->tableCardNodeTop7Pos = {};
    for (int i = 1; i <= 7; i++)
    {
        FVector2D mPos = mUIRoot->GetWidgetFromName("Top7Pos" + i)->GetRenderTransform().Translation;
        this->tableCardNodeTop7Pos.Add(mPos);
    }

    this->PokerItemParent = Cast<UCanvasPanel>(mUIRoot->GetWidgetFromName("PokerItemParent"));

}

void UMainUIWidget::Show()
{
    this->Init();
    this->SetVisibility(ESlateVisibility::Visible);

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
    //local tableOpStepItem = RecordStepDataHandler.data.tableOpStepItem
    //    local mInitSendCardList = RecordStepDataHandler.data.mInitSendCardList
    //    if  RecordStepDataHandler:orGameEnd() then
    //        self : NewGameBegin_ForNormal(true)
    //            return
    //        end
    //        self.mLastSendCardList = mInitSendCardList
    //        -------------------------- - 初始化资源------------------------------
    this->RecycleAndInitCardGo();
    this->bGameEnd = false;
    //this->nGameMode = RecordStepDataHandler.data.nGameMode
    this->PokerItemParent->SetVisibility(ESlateVisibility::Collapsed);
    
    ensureMsgf(mInitSendCardList.Num() == 52, TEXT("mInitSendCardList Error: %d"), mInitSendCardList.Num());
    for (int i = 1; i < mInitSendCardList.Num(); i++)
    {
        this->mSendCardListGo[i]->SetPokerId(mInitSendCardList[i]);
        this->mSendCardListGo[i]->SetTurnOverState(false);
    }

    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j < i; j++)
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
    //TODO 这里就是恢复逻辑

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

    this->RecoverGame(true);
}

void UMainUIWidget::RecoverGame(bool bPlayAni)
{
    if (bPlayAni)
    {
        for (int i = 1; i <= 7; i++)
        {
            for (int j = 1; j < this->tableCardNodeTop7Go[i].Num(); j++)
            {
                auto mCardItem = this->tableCardNodeTop7Go[i][j];
                /* mCardItem.transform:SetAsLastSibling()
                 mCardItem : Show()
                 mCardItem : ForceShowBackUI()
                 mCardItem : SetEventTriggerState(mCardItem : orTurnOverStateIsTrue())
                 local fromPos = self : GetCardNodeSendPokerPos()
                 mCardItem.transform.localPosition = fromPos
                 end
                 end

                 for i = 1, 4 do
                     for j = 1, #self.tableCardNode4AGo[i] do
                         local mCardItem = self.tableCardNode4AGo[i][j]
                         mCardItem.transform:SetAsLastSibling()
                         mCardItem : ForceShowBackUI()
                         mCardItem : Show()
                         mCardItem : SetEventTriggerState(j == #self.tableCardNode4AGo[i])
                         local fromPos = self : GetCardNodeSendPokerPos()
                         mCardItem.transform.localPosition = fromPos
                         end
                         end

                         for i = 1, #self.tableCardDraw3Go do
                             local mCardItem = self.tableCardDraw3Go[i]
                             mCardItem.transform:SetAsFirstSibling()
                             mCardItem : ForceShowBackUI()
                             mCardItem : SetEventTriggerState(i == 1)

                             if i <= 3 then
                                 mCardItem : Show()
                             else
                                 mCardItem : Hide()
                                 end

                                 local fromPos = self : GetCardNodeSendPokerPos()
                                 mCardItem.transform.localPosition = fromPos
                                 end

                                 ------------------ - 动画播放-------------------- -
                                 AudioHandler : PlaySound("start_new")
                                 for i = 1, 7 do
                                     for j = 1, #self.tableCardNodeTop7Go[i] do
                                         local mCardItem = self.tableCardNodeTop7Go[i][j]
                                         local fromPos = self:GetCardNodeSendPokerPos()
                                         local toPos = self : GetCardNodeTop7Pos(i, j)
                                         local mTween = TweenMgr : AddGoTween(self.transform, 0.3, function(fTimePercent)
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
            }
        }
    }
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