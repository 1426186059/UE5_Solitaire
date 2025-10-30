// Fill out your copyright notice in the Description page of Project Settings.


#include "UKKGameInstance.h"

void UKKGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("=== MyGameInstance Init ==="));
    
    // 任意游戏世界（PIE / Standalone）创建完成后都会回调
    FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UKKGameInstance::OnWorldInitialized);
    //FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UKKGameInstance::OnWorldCreated);
}

void UKKGameInstance::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("=== MyGameInstance Shutdown ==="));
    Super::Shutdown();
}

void UKKGameInstance::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
    UE_LOG(LogTemp, Error, TEXT("UKKGameInstance OnWorldInitialized: %s"), *UEnum::GetValueAsString(World->WorldType));
    if (!World || (World->WorldType != EWorldType::Game && World->WorldType != EWorldType::PIE))
    {
        return;
    }

    FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
    if (GEngine->GetWorld())
    {
        AGameLauncher::GetSingleton()->Init();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnWorldInitialized Error"));
        ensureMsgf(Get(), TEXT(""));
    }
}

void UKKGameInstance::OnWorldCreated(UWorld* World)
{
    UE_LOG(LogTemp, Error, TEXT("UKKGameInstance OnWorldCreated: %s"), *UEnum::GetValueAsString(World->WorldType));
    if (!World || (World->WorldType != EWorldType::Game && World->WorldType != EWorldType::PIE))
    {
        return;
    }

    FWorldDelegates::OnPostWorldCreation.RemoveAll(this);

    if (GEngine->GetWorld())
    {
        AGameLauncher::GetSingleton()->Init();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnWorldInitialized Error"));
        ensureMsgf(Get(), TEXT(""));
    }
}

