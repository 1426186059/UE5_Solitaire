// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLauncher.h"

AGameLauncher::AGameLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGameLauncher::BeginPlay()
{
	Super::BeginPlay();

    //设置目标帧率
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        UserSettings->SetFrameRateLimit(60);
        UserSettings->ApplySettings(false);
        UserSettings->SaveSettings();
    }

    //指定照相机
    //APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    //if (PC != NULL)
    //{
    //    for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
    //    {
    //        ACameraActor* Actor = *It;
    //        //if (Actor->GetActorLabel() == "CameraActor")
    //        {
    //            PC->SetViewTarget(Actor);
    //        }
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Log, TEXT("PC IS NULL"));
    //}

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_InitSceneDoFinishOK)->AddUObject(this, &AGameLauncher::GoToLobby);
    
    //加载InitScene界面
    TSubclassOf<UInitSceneWidget> BPClass = LoadClass<UInitSceneWidget>(nullptr,
        TEXT("/Game/ResourceABs/InitScene/BPS/IntSceneCWBP.IntSceneCWBP_C"));
    if (BPClass != NULL)
    {
        UInitSceneWidget* mUInitSceneWidget = CreateWidget<UInitSceneWidget>(GEngine->GameViewport->GetWorld(), BPClass);
        mUInitSceneWidget->Show();
    }

    //记载一些数据
    DataCenter::GetSingleton()->Init();
}

void AGameLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGameLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameLauncher::GoToLobby(void* param)
{
    UE_LOG(LogTemp, Log, TEXT("AGameLauncher GoToLobby"));

    TSubclassOf<UMainUIWidget> BPClass = LoadClass<UMainUIWidget>(nullptr,
        TEXT("/Game/ResourceABs/MainScene/BPS/MainUICWBP.MainUICWBP_C"));
    if (BPClass != NULL)
    {
        UMainUIWidget* mUInitSceneWidget = CreateWidget<UMainUIWidget>(GEngine->GameViewport->GetWorld(), BPClass);
        mUInitSceneWidget->Show();
    }
}

