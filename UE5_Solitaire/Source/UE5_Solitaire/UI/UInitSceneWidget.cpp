// Fill out your copyright notice in the Description page of Project Settings.


#include "UInitSceneWidget.h"

void UInitSceneWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 方式 1：通过名称查找（推荐，无需 BindWidget）
    UProgressBar* ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));
    if (!ProgressBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find ProgressBar by name 'ProgressBar'!"));
    }
}

void UInitSceneWidget::SetLoadProgress(float Percent01)
{
   /* if (PB_Main)
        PB_Main->SetPercent(FMath::Clamp(Percent01, 0.f, 1.f));*/
}