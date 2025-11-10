// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMGHelper.h"
#include "UE5_Solitaire/SimpleFramework/KKTween/KKTweenExtentions.h"

class UMGAni
{
public:
    static void PlayShowRightToLeftAni(UUserWidget* viewNode, bool bShow, TFunction<void()> finishFunc = nullptr);
    static void PlayShowScaleAni(UUserWidget* viewNode, bool bShow, TFunction<void()> finishFunc = nullptr);
    static void PlayShowAlphaAni(UUserWidget* viewNode, bool bShow, TFunction<void()> finishFunc = nullptr);
    static void PlayShowDownToUpAni(UUserWidget* viewNode, bool bShow, TFunction<void()> finishFunc = nullptr);
private:
    UMGAni() = delete;
    ~UMGAni() = delete;
    UMGAni(const UMGAni&) = delete;
    UMGAni& operator=(const UMGAni&) = delete;
    UMGAni(UMGAni&&) = delete;
    UMGAni& operator=(UMGAni&&) = delete;
};
