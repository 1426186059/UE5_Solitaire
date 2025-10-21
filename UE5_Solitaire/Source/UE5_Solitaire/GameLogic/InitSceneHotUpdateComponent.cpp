// Fill out your copyright notice in the Description page of Project Settings.


#include "InitSceneHotUpdateComponent.h"

// Sets default values
UInitSceneHotUpdateComponent::UInitSceneHotUpdateComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UInitSceneHotUpdateComponent::BeginPlay()
{
	Super::BeginPlay();
    //this->RequestAllPrimaryAsset();
    this->RequestLoadAllRes();
    UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent BeginPlay"));
}

// 每帧 Tick（或定时器）里轮询
void UInitSceneHotUpdateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (mFStreamableHandle.IsValid() && mFStreamableHandle->IsLoadingInProgress())
    {
        float Percent = mFStreamableHandle->GetProgress();          // 0.0~1.0
        int32 Loaded, Requested;
        mFStreamableHandle->GetLoadedCount(Loaded, Requested);    // 已加载 / 总量
        UE_LOG(LogTemp, Log, TEXT("UI AInitSceneMgr Loading: %.0f (%d / %d)"), Percent * 100.f, Loaded, Requested);
    }
}

void UInitSceneHotUpdateComponent::RequestLoadAllRes()
{
    TArray<FSoftObjectPath> AssetsToLoad;
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/BPS/MainUIBP")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card1")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card2")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card3")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card4")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card5")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card6")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card7")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card8")));
    AssetsToLoad.Add(FSoftObjectPath(TEXT("/Game/ResourceABs/MainScene/AtlasGroup/card9")));

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    TSharedPtr<FStreamableHandle> Handle = Streamable.RequestAsyncLoad(
        AssetsToLoad,
        FStreamableDelegate::CreateUObject(this, &UInitSceneHotUpdateComponent::OnAssetLoadCompleted)
    );
}

// 开始加载
void UInitSceneHotUpdateComponent::RequestAllPrimaryAsset()
{
    TArray<FPrimaryAssetId> AssetsToLoad;
    AssetsToLoad.Add(FPrimaryAssetId(FName("UIAsset"), FName("DA_MainHUD")));
    
    TArray<FName> LoadBundles;
    LoadBundles.Add(FName("UI"));

    UAssetManager& AM = UAssetManager::Get();
    mFStreamableHandle = AM.LoadPrimaryAssets(
        AssetsToLoad,
        LoadBundles,
        FStreamableDelegate::CreateUObject(this, &UInitSceneHotUpdateComponent::OnAssetLoadCompleted),
        FStreamableManager::DefaultAsyncLoadPriority
    );
}

void UInitSceneHotUpdateComponent::OnAssetLoadCompleted()
{
    UE_LOG(LogTemp, Log, TEXT("所有UI资产加载完成"));
    mFStreamableHandle.Reset();
}

