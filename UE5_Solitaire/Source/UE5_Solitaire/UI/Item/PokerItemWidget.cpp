// Fill out your copyright notice in the Description page of Project Settings.

#include "PokerItemWidget.h"

void UPokerItemWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    this->mUIRoot = this;
    this->mIcon = Cast<UImage>(this->mUIRoot->GetWidgetFromName(TEXT("Icon")));
    if (!mIcon)
    {
        UE_LOG(LogTemp, Error, TEXT("mIcon == null"));
        return;
    }

    //UButton* mGameNodeBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("gameNodeBtn"));
    //mGameNodeBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_GameNodeBtn);
    //UWidget* mFaPaiPos = mUIRoot->GetWidgetFromName("FaPaiPos");
}

void UPokerItemWidget::Show()
{
    this->Init();
    this->SetVisibility(ESlateVisibility::Visible);
}

void UPokerItemWidget::Hide()
{
    this->SetVisibility(ESlateVisibility::Collapsed);
}

void UPokerItemWidget::Refresh()
{
    this->Init();
    if (this->orTurnOverStateIsTrue())
    {
        this->mIcon->SetBrushFromAtlasInterface(AResCenter::GetSingleton()->GetPokerSprite(this->nPokerId));
    }
    else
    {
        this->mIcon->SetBrushFromAtlasInterface(AResCenter::GetSingleton()->GetPokerSprite(-1));
    }
}

void UPokerItemWidget::SetPokerId(int nPokerId1)
{
    this->nPokerId = nPokerId1;
}

void UPokerItemWidget::SetTurnOverState(bool bShow, int nStepIndex)
{
    //if self.bTurnOverState == bShow then return end
    //self.bTurnOverState = bShow
    //if bShow then
    //if nStepIndex == nil then
    //nStepIndex = GameView:GetNowRecordStepIndex()
    //end

    //self.nStepIndex_ForFirstShowPokerId = nStepIndex
    //else
    //self.nStepIndex_ForFirstShowPokerId = -1
    //end
}

bool UPokerItemWidget::orTurnOverStateIsTrue()
{
    return this->bTurnOverState;
}

void UPokerItemWidget::ForceShowBackUI()
{
    this->mIcon->SetBrushFromAtlasInterface(AResCenter::GetSingleton()->GetPokerSprite(-1));
}

void UPokerItemWidget::SetEventTriggerState(bool bCanClick)
{
    if (this->IsFocusable() != bCanClick)
    {
        this->SetIsFocusable(bCanClick);
    }
}