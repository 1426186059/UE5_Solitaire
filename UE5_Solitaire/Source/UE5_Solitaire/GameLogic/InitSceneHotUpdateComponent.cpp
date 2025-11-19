// Fill out your copyright notice in the Description page of Project Settings.


#include "InitSceneHotUpdateComponent.h"

//#include "HAL/PlatformFilemanager.h"
//#include "Misc/FileHelper.h"
//#include "Serialization/MemoryReader.h"
//#include "Serialization/MemoryWriter.h"
//#include "Internationalization/TextPackageNamespaceUtil.h"
#include <IPlatformFilePak.h>

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
    this->LoadAndMountPak();
    this->RequestLoadAllRes();
}

// 每帧 Tick（或定时器）里轮询
void UInitSceneHotUpdateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (mFStreamableHandle.IsValid())
    {
        if (mFStreamableHandle->IsLoadingInProgress())
        {
            float Percent = mFStreamableHandle->GetProgress();          // 0.0~1.0
            int32 Loaded, Requested;
            mFStreamableHandle->GetLoadedCount(Loaded, Requested);    // 已加载 / 总量
            this->UpdateProgressFunc->Broadcast(Percent);

            UE_LOG(LogTemp, Log, TEXT("UI AInitSceneMgr Loading: %.0f (%d / %d)"), Percent, Loaded, Requested);
        }

        if (mFStreamableHandle->HasLoadCompleted())
        {
            this->UpdateProgressFunc->Broadcast(1.0f);
        }
    }
}

void UInitSceneHotUpdateComponent::LoadAndMountPak()
{
    LoadAndMountPakFromStreamingAssets("hot_update_mainscene.pak");
}

void UInitSceneHotUpdateComponent::LoadAndMountPakFromStreamingAssets(const FString& pakName)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    FString SourcePakPath = FPaths::Combine(*FPaths::ProjectContentDir(), TEXT("StreamingAssets/HotUpdatePak/"), pakName);
    FString DestPakPath = FPaths::Combine(*FPaths::ProjectSavedDir(), TEXT("HotUpdatePak/"), pakName);

    if (!PlatformFile.FileExists(*DestPakPath))
    {
        UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent Copying PAK from %s to %s"), *SourcePakPath, *DestPakPath);
        if (UEHelper::SafeCopyFile(*DestPakPath, *SourcePakPath))
        {
            UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent Successfully copied PAK file."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UInitSceneHotUpdateComponent Failed to copy PAK file!"));
            return;
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent PAK file already exists at destination. Skipping copy."));
    }
    
    FPakPlatformFile* PakFileInterface = UEHelper::GetFPakPlatformFile();
    if (PakFileInterface)
    {
        const int32 PakPriority = 0;
        const FString MountPoint = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
        if (PakFileInterface->Mount(*DestPakPath, PakPriority, *MountPoint))
        {
            UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent Successfully mounted PAK: %s"), *DestPakPath);
            UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent Content is now accessible under: %s"), *MountPoint);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UInitSceneHotUpdateComponent Failed to mount PAK: %s"), *DestPakPath);
        }
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
    mFStreamableHandle = Streamable.RequestAsyncLoad(
        AssetsToLoad,
        FStreamableDelegate::CreateUObject(this, &UInitSceneHotUpdateComponent::OnAssetLoadCompleted)
    );
}

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
    UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent Finish Ok"));
    mFStreamableHandle.Reset();
    this->UpdateFinishFunc->Broadcast();
}

