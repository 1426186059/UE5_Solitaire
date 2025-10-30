// Fill out your copyright notice in the Description page of Project Settings.


#include "UKKGameInstance.h"

UKKGameInstance* UKKGameInstance::mInstance = nullptr;

void UKKGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("=== MyGameInstance Init ==="));
    
    // 任意游戏世界（PIE / Standalone）创建完成后都会回调
    //FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UKKGameInstance::OnWorldInitialized);
    //FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UKKGameInstance::OnWorldCreated);
}

void UKKGameInstance::Shutdown()
{
    Super::Shutdown();
    UE_LOG(LogTemp, Log, TEXT("=== MyGameInstance Shutdown ==="));
}

void UKKGameInstance::OnStart()
{
    Super::OnStart();
    UE_LOG(LogTemp, Log, TEXT("=== MyGameInstance OnStart ==="));
    this->CreateGameLauncher();
}

void UKKGameInstance::CreateGameLauncher()
{
    this->GetWorld()->SpawnActor<AGameLauncher>();
}

void UKKGameInstance::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
    UE_LOG(LogTemp, Error, TEXT("UKKGameInstance OnWorldInitialized: %d"), World->WorldType.GetValue());
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
    }
}

void UKKGameInstance::OnWorldCreated(UWorld* World)
{
    UE_LOG(LogTemp, Error, TEXT("UKKGameInstance OnWorldCreated: %d"), World->WorldType.GetValue());
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
    }
}

