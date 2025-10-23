// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"

void UMainUIWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMainUIWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UMainUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainUIWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    mUIRoot = Cast<UUserWidget>(GetWidgetFromName(TEXT("MainUIWBP")));
    if (!mUIRoot)
    {
        UE_LOG(LogTemp, Error, TEXT("mUIRoot == null"));
    }

    this->AddToViewport(0);

    auto mBG = Cast<UImage>(mUIRoot->GetWidgetFromName(TEXT("BG")));
    if (!mBG)
    {
        UE_LOG(LogTemp, Error, TEXT("mBG == null"));
    }

    UMGAdapterTool::GetSingleton()->FitBG(mBG);
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