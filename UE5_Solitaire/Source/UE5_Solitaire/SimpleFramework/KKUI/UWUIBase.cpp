// Fill out your copyright notice in the Description page of Project Settings.


#include "UWUIBase.h"

void UWUIBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FVector2D mSize = UMGHelper::GetUMGRootSzie(this);
    if (mSize != mOldSize && mSize != FVector2D::ZeroVector)
    {
        mOldSize = mSize;
        UE_LOG(LogTemp, Log, TEXT("UWUIBase GetUMGRootSzie: %s"), *mSize.ToString());
        this->OnLayoutChanged();
    }
}

bool UWUIBase::orFirstLayoutFinish()
{
    return this->bFirstLayoutFinish;
}

bool UWUIBase::orInit()
{
    return this->bInit;
}

void UWUIBase::OnCreate()
{

}

void UWUIBase::Init()
{
    
}

void UWUIBase::Show()
{
    this->bShowUI = true;
    this->bFirstLayoutFinish = UMGHelper::GetUMGRootSzie(this) != FVector2D::ZeroVector;
    this->CheckFirstLayoutOkToShow();
}

void UWUIBase::Hide(bool bDestroy)
{
    this->bShowUI = false;
    this->SetVisibility(ESlateVisibility::Hidden);
    
    if (bDestroy)
    {
        UMGHelper::DestroyWidget(this);
    }
}

void UWUIBase::Refresh()
{
    
}

void UWUIBase::OnLayoutChanged()
{
    if (!this->bFirstLayoutFinish)
    {
        this->bFirstLayoutFinish = true;
        this->CheckFirstLayoutOkToShow();
    }
}

void UWUIBase::CheckFirstLayoutOkToShow()
{
    if (orFirstLayoutFinish())
    {
        if (this->bShowUI)
        {
            this->SetVisibility(ESlateVisibility::Visible);
        }
    }
}