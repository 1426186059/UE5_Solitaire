// Fill out your copyright notice in the Description page of Project Settings.


#include "InitSceneHotUpdateMgr.h"

// Sets default values
UInitSceneHotUpdateMgr::UInitSceneHotUpdateMgr()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UInitSceneHotUpdateMgr::BeginPlay()
{
	Super::BeginPlay();
    this->LoadUIAsset(FPrimaryAssetId(FName("UIAsset"), FName("DA_MainHUD")));
    this->RequestLoadAllRes();
}

// 每帧 Tick（或定时器）里轮询
void UInitSceneHotUpdateMgr::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (mFStreamableHandle.IsValid() && mFStreamableHandle->IsLoadingInProgress())
    {
        float Percent = mFStreamableHandle->GetProgress();          // 0.0~1.0
        int32 Loaded, Requested;
        mFStreamableHandle->GetLoadedCount(Loaded, Requested);    // 已加载 / 总量
        UE_LOG(LogTemp, Log, TEXT("UI AInitSceneMgr Loading: %.0f %%(%d / %d)"), Percent * 100.f, Loaded, Requested);
    }
}

void UInitSceneHotUpdateMgr::RequestLoadAllRes()
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
        FStreamableDelegate::CreateLambda([]() 
            {
                UE_LOG(LogTemp, Log, TEXT("All assets loaded!"));
            })
    );
}

// 开始加载
void UInitSceneHotUpdateMgr::LoadUIAsset(FPrimaryAssetId AssetId)
{
    UAssetManager& AM = UAssetManager::Get();
    mFStreamableHandle = AM.LoadPrimaryAsset(
        AssetId, 
        TArray<FName>{TEXT("UI")},
        FStreamableDelegate::CreateUObject(this, &UInitSceneHotUpdateMgr::OnAssetLoadCompleted),
        FStreamableManager::DefaultAsyncLoadPriority
    );
}

void UInitSceneHotUpdateMgr::OnAssetLoadCompleted()
{
    UE_LOG(LogTemp, Log, TEXT("=== 所有 UI 资产加载完成 ==="));
    mFStreamableHandle.Reset();
}

