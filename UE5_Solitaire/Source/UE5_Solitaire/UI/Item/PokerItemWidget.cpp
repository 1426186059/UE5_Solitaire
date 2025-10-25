// Fill out your copyright notice in the Description page of Project Settings.

#include "PokerItemWidget.h"

void UPokerItemWidget::Init()
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

    //UButton* mGameNodeBtn = Cast<UButton>(mUIRoot->GetWidgetFromName("gameNodeBtn"));
    //mGameNodeBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_GameNodeBtn);
    //UWidget* mFaPaiPos = mUIRoot->GetWidgetFromName("FaPaiPos");
}

void UPokerItemWidget::Show()
{
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