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
    FTransform SpawnTM(FRotator::ZeroRotator, FVector(0, 0, 100));
    AInitSceneHotUpdateMgr* NewActor = GetWorld()->SpawnActor<AInitSceneHotUpdateMgr>(AInitSceneHotUpdateMgr::StaticClass(), SpawnTM);
    if (NewActor)
    {
#if UE_EDITOR
        NewActor->SetActorLabel(TEXT("AInitSceneHotUpdateMgr"));
#endif
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NewActor IS NULL"));
    }
}

void UInitSceneWidget::Hide()
{
    this->SetVisibility(ESlateVisibility::Collapsed);
}

void UInitSceneWidget::Refresh()
{
    mUProgressBar->SetPercent(0);
}

void UInitSceneWidget::SetLoadProgress(float Percent01)
{
   /* if (PB_Main)
        PB_Main->SetPercent(FMath::Clamp(Percent01, 0.f, 1.f));*/
}