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

    UWidget* mFaPaiPos = mUIRoot->GetWidgetFromName("FaPaiPos");
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
    //BG   ≈‰∆¡ƒª
    auto mBG = Cast<UImage>(mUIRoot->GetWidgetFromName(TEXT("BG")));
    UMGAdapterTool::GetSingleton()->FitBG(mUIRoot, mBG);
}

void UMainUIWidget::OnBtnClicked_GameNodeBtn()
{
    UE_LOG(LogTemp, Log, TEXT("UMainUIWidget OnBtnClicked_GameNodeBtn"));
}

void UMainUIWidget::InitGame()
{
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

}

void UMainUIWidget::RecoverGame(bool bPlayAni)
{
    if (bPlayAni)
    {
        for (int i = 1; i <= 7; i++)
        {
            /*for j = 1, #self.tableCardNodeTop7Go[i] do
                local mCardItem = self.tableCardNodeTop7Go[i][j]
                mCardItem.transform:SetAsLastSibling()
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

                                ------------------ - ∂Øª≠≤•∑≈-------------------- -
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