// Fill out your copyright notice in the Description page of Project Settings.


#include "KKUserWidgetBase.h"

void UKKUserWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FVector2D mSize = UMGHelper::GetUMGRootSzie(this);
    if (mSize != mOldSize && mSize != FVector2D::ZeroVector)
    {
        mOldSize = mSize;
        this->OnLayoutChanged();

        UE_LOG(LogTemp, Log, TEXT("UKKUserWidgetBase GetUMGRootSzie: %s"), *mSize.ToString());
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
    if (!this->IsInViewport())
    {
        this->AddToViewport(0);
    }

    this->SetVisibility(ESlateVisibility::Visible);
    this->bShowUI = true;
    this->CheckFirstLayoutOkToInit();
}

void UKKUserWidgetBase::Hide(bool bDestroy)
{
    this->bShowUI = false;
    this->SetVisibility(ESlateVisibility::Hidden);
    
    if (bDestroy)
    {
        if (this->IsInViewport())
        {
            this->RemoveFromParent();
        }
    }
}

void UKKUserWidgetBase::Refresh()
{
    
}

void UKKUserWidgetBase::OnLayoutChanged()
{
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