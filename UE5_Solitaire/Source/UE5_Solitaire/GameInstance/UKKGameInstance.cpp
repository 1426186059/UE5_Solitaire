// Fill out your copyright notice in the Description page of Project Settings.


#include "UKKGameInstance.h"

void UKKGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("=== MyGameInstance Init ==="));
    
    this->GetActorSingleton<AGameLauncher>();

    // 任意游戏世界（PIE / Standalone）创建完成后都会回调
    //FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UKKGameInstance::OnWorldInitialized);
}

void UKKGameInstance::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("=== MyGameInstance Shutdown ==="));
    Super::Shutdown();
}

void UKKGameInstance::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
    // 只关心真正的游戏世界
    if (!World || !World->IsGameWorld()) return;

    // 现在世界已完全准备好，可以安全 Spawn
    //创建热更Actor
    this->GetActorSingleton<AGameLauncher>();
}

