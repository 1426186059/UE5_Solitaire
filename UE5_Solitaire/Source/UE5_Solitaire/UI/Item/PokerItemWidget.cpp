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
    //mUProgressBar->SetPercent(0);
}