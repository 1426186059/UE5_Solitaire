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

    mUProgressBar->SetPercent(0);

    //创建热更Actor
    mInitSceneHotUpdateMgr = GetWorld()->SpawnActor<AInitSceneHotUpdateMgr>(AInitSceneHotUpdateMgr::StaticClass());
    mInitSceneHotUpdateMgr->UpdateProgressFunc.AddUObject(this, &UInitSceneWidget::UpdateProgressFunc);
    mInitSceneHotUpdateMgr->UpdateFinishFunc.AddUObject(this, &UInitSceneWidget::UpdateFinishFunc);
    mInitSceneHotUpdateMgr->UpdateErrorFunc.AddUObject(this, &UInitSceneWidget::UpdateErrorFunc);
    mInitSceneHotUpdateMgr->UpdateVersionFunc.AddUObject(this, &UInitSceneWidget::UpdateVersionFunc);
}

void UInitSceneWidget::Hide()
{
    this->SetVisibility(ESlateVisibility::Collapsed);

    mInitSceneHotUpdateMgr->UpdateProgressFunc.RemoveAll(this);
    mInitSceneHotUpdateMgr->UpdateFinishFunc.RemoveAll(this);
    mInitSceneHotUpdateMgr->UpdateErrorFunc.RemoveAll(this);
    mInitSceneHotUpdateMgr->UpdateVersionFunc.RemoveAll(this);
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
    UE_LOG(LogTemp, Error, TEXT("Update Finish Func"));

    mInitSceneHotUpdateMgr->Destroy();
    mInitSceneHotUpdateMgr = nullptr;

    this->Hide();
    if (this->IsInViewport())
    {
        this->RemoveFromParent();
    }
}

void UInitSceneWidget::UpdateErrorFunc()
{
    UE_LOG(LogTemp, Error, TEXT("UpdateErrorFunc"));
}

void UInitSceneWidget::UpdateVersionFunc()
{
    UE_LOG(LogTemp, Error, TEXT("Update Version Func"));
}