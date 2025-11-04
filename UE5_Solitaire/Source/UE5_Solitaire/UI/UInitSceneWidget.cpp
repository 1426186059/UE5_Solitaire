// Fill out your copyright notice in the Description page of Project Settings.


#include "UInitSceneWidget.h"

void UInitSceneWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInitSceneWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInitSceneWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    mUIRoot = Cast<UUserWidget>(GetWidgetFromName(TEXT("InitSceneUIBP")));
    if (!mUIRoot)
    {
        UE_LOG(LogTemp, Error, TEXT("mUIRoot == null"));
    }

    mUProgressBar = Cast<UProgressBar>(mUIRoot->GetWidgetFromName(TEXT("ProgressBar_139")));
    if (!mUProgressBar)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find ProgressBar by name 'ProgressBar'!"));
    }

    this->AddToViewport(0);
}

void UInitSceneWidget::Show()
{
    this->Init();
    this->SetVisibility(ESlateVisibility::Visible);

    this->mUProgressBar->SetPercent(0);

    //创建热更Actor
    mInitSceneHotUpdateMgr = GetWorld()->SpawnActor<AInitSceneHotUpdateMgr>(AInitSceneHotUpdateMgr::StaticClass());
    mInitSceneHotUpdateMgr->UpdateProgressFunc.AddUObject(this, &UInitSceneWidget::UpdateProgressFunc);
    mInitSceneHotUpdateMgr->UpdateFinishFunc.AddUObject(this, &UInitSceneWidget::UpdateFinishFunc);;
    mInitSceneHotUpdateMgr->UpdateErrorFunc.AddUObject(this, &UInitSceneWidget::UpdateErrorFunc);
    mInitSceneHotUpdateMgr->UpdateVersionFunc.AddUObject(this, &UInitSceneWidget::UpdateVersionFunc);
}

void UInitSceneWidget::Hide(bool bDestory)
{
    this->SetVisibility(ESlateVisibility::Collapsed);

    if (mInitSceneHotUpdateMgr != nullptr)
    {
        mInitSceneHotUpdateMgr->UpdateProgressFunc.RemoveAll(this);
        mInitSceneHotUpdateMgr->UpdateFinishFunc.RemoveAll(this);
        mInitSceneHotUpdateMgr->UpdateErrorFunc.RemoveAll(this);
        mInitSceneHotUpdateMgr->UpdateVersionFunc.RemoveAll(this);
    }

    if (bDestory)
    {
        if (this->IsInViewport())
        {
            this->RemoveFromParent();
        }
    }
}

void UInitSceneWidget::Refresh()
{
    mUProgressBar->SetPercent(0);
}

void UInitSceneWidget::UpdateProgressFunc(float Percent01)
{
    mUProgressBar->SetPercent(Percent01);
}

void UInitSceneWidget::UpdateFinishFunc()
{
    UE_LOG(LogTemp, Log, TEXT("UInitSceneWidget UpdateFinishFunc"));

    if (mInitSceneHotUpdateMgr)
    {
        mInitSceneHotUpdateMgr->Destroy(); //避免在委托回调中修改委托本身（推荐） ，导致的崩溃
        mInitSceneHotUpdateMgr = nullptr;
    }
    
    KKEventMgr::GetSingleton()->Broadcast(GameConst::EventId_InitSceneDoFinishOK, nullptr);
}

void UInitSceneWidget::UpdateErrorFunc()
{
    UE_LOG(LogTemp, Error, TEXT("UInitSceneWidget UpdateErrorFunc"));
}

void UInitSceneWidget::UpdateVersionFunc()
{
    UE_LOG(LogTemp, Error, TEXT("UInitSceneWidget UpdateVersionFunc"));
}