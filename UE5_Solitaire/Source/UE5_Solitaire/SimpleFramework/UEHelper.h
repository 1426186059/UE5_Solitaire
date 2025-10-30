// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UEHelper
{
public:
    static UWorld* GetKKWorld()
    {
        auto mWorld = GEngine->GameViewport->GetWorld();
        if (mWorld == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UEHelper GetKKWorld == null"));
        }

        return mWorld;
    }
private:
    // 禁止拷贝和移动（单例不应被复制）
    UEHelper() = delete;
    ~UEHelper() = delete;
    UEHelper(const UEHelper&) = delete;
    UEHelper& operator=(const UEHelper&) = delete;
    UEHelper(UEHelper&&) = delete;
    UEHelper& operator=(UEHelper&&) = delete;
};
