// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate/SceneViewport.h"

class UEHelper
{
public:
    static FIntPoint GetScreenResolution()
    {
        if (GEngine && GEngine->GameViewport)
        {
            if (FSceneViewport* VP = GEngine->GameViewport->GetGameViewport())
            {
                return VP->GetSize();        // 真实像素
            }
        }
        return FIntPoint::ZeroValue;
    }


    static FIntPoint GetScreenSize()
    {
        if (GEngine && GEngine->GetGameUserSettings())
        {
            return GEngine->GetGameUserSettings()->GetScreenResolution();
        }
        return FIntPoint::ZeroValue;
    }

private:
    // 禁止拷贝和移动（单例不应被复制）
    UEHelper(const UEHelper&) = delete;
    UEHelper& operator=(const UEHelper&) = delete;
    UEHelper(UEHelper&&) = delete;
    UEHelper& operator=(UEHelper&&) = delete;

};
