// Fill out your copyright notice in the Description page of Project Settings.


#include "KKUserWidgetBase.h"

void UKKUserWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FVector2D mSize = UMGHelper::GetUMGRootSzie(this);
    if (mSize != mOldSize)
    {
        mOldSize = mSize;
        this->OnScreenSizeChanged();
    }
}

bool UKKUserWidgetBase::orFirstLayoutFinish()
{
    return this->bFirstLayoutFinish;
}

bool UKKUserWidgetBase::orInit()
{
    return this->bInit;
}

void UKKUserWidgetBase::Init()
{
    this->bInit = true;
}

void UKKUserWidgetBase::Show()
{
    this->SetVisibility(ESlateVisibility::Visible);
    this->bShowUI = true;
    this->CheckFirstLayoutOkToInit();
}

void UKKUserWidgetBase::Hide()
{
    this->bShowUI = false;
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UKKUserWidgetBase::Refresh()
{
    
}

void UKKUserWidgetBase::OnScreenSizeChanged()
{
    UE_LOG(LogTemp, Log, TEXT("OnScreenSizeChanged  OnLayoutFinish"));
    if (!this->bFirstLayoutFinish)
    {
        this->bFirstLayoutFinish = true;
        this->OnFirstLayoutFinish();
    }
}

void UKKUserWidgetBase::OnFirstLayoutFinish()
{
    this->CheckFirstLayoutOkToInit();
}

void UKKUserWidgetBase::CheckFirstLayoutOkToInit()
{

}