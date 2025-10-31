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

    static double GetTime_Time(bool realTime = false)
    {
        if (realTime)
        {
            return FApp::GetCurrentTime();
        }
        else
        {
            return GetKKWorld()->GetTimeSeconds();
        }
    }

    static double GetTime_DeltaTime(bool realTime = false)
    {
        if (realTime)
        {
            return FApp::GetDeltaTime();
        }
        else
        {
            return GetKKWorld()->GetDeltaSeconds();
        }
    }

    template<typename T, typename FuncType>
    static void RunNextFrame(T* Obj, FuncType&& Func)
    {
        FTimerHandle Temp;
        GetKKWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(Obj, Forward<FuncType>(Func)));
    }
    
    static void RunNextFrame(TFunction<void()> Func)
    {
        FTimerHandle Temp;
        GetKKWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda(MoveTemp(Func)));
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
