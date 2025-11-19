// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserInterfaceSettings.h"
#include "Kismet/KismetSystemLibrary.h"

class UEHelper
{
public:
    static UWorld* GetKKWorld(UObject* obj = nullptr)
    {
        if (obj)
        {
            return obj->GetWorld();
        }
        else
        {
            auto mWorld = GEngine->GameViewport->GetWorld();
            if (mWorld == nullptr)
            {
                UE_LOG(LogTemp, Error, TEXT("UEHelper GetKKWorld == null"));
            }
            return mWorld;
        }
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

    static void StartTimer(UObject* obj, FTimerHandle mHandle)
    {
        GetKKWorld(obj)->GetTimerManager().UnPauseTimer(mHandle);
    }

    static void PauseTimer(UObject* obj, FTimerHandle mHandle)
    {
        GetKKWorld(obj)->GetTimerManager().PauseTimer(mHandle);
    }

    static bool SafeCopyFile(const FString& Dst, const FString& Src)
    {
        // 1. 建目录
        FString DstDir = FPaths::GetPath(Dst);
        IPlatformFile& Pf = FPlatformFileManager::Get().GetPlatformFile();
        if (!Pf.DirectoryExists(*DstDir))
        {
            Pf.CreateDirectoryTree(*DstDir);
        }

        // 3. 真正拷贝
        return Pf.CopyFile(*Dst, *Src);
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
