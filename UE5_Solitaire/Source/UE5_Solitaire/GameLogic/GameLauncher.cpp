// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLauncher.h"

AGameLauncher::AGameLauncher()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameLauncher::BeginPlay()
{
	Super::BeginPlay();

    //一些测试例子
    Test::Do();

    //设置目标帧率
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        UserSettings->SetFrameRateLimit(60);
        UserSettings->ApplySettings(false);
        UserSettings->SaveSettings();
    }
    
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        //隐藏手机界面 2个虚拟摇杆。
        PC->ActivateTouchInterface(nullptr);   // 把整个 Touch Interface 卸掉
        PC->SetVirtualJoystickVisibility(false);

        //显示鼠标
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock));
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

    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_InitSceneDoFinishOK)->AddUObject(this, &AGameLauncher::StartEnterGame);
    
    //加载InitScene界面
    TSubclassOf<UInitSceneWidget> BPClass = LoadClass<UInitSceneWidget>(nullptr,
        TEXT("/Game/ResourceABs/InitScene/BPS/IntSceneCWBP.IntSceneCWBP_C"));
    if (BPClass != NULL)
    {
        mUInitSceneWidget = CreateWidget<UInitSceneWidget>(GEngine->GameViewport->GetWorld(), BPClass);
        mUInitSceneWidget->Show();
    }
}

void AGameLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGameLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameLauncher::StartEnterGame(void* param)
{
    UE_LOG(LogTemp, Log, TEXT("AGameLauncher StartEnterGame"));
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_InitSceneDoFinishOK)->RemoveAll(this);

    //加载CSV配置表
    CSVConfigMgr::GetSingleton()->Init();
    DTMgr::GetSingleton()->Init();

    //加载游戏数据
    DataCenter::GetSingleton()->Init(); //改为同步调用了
    CardHandler::GetSingleton()->Init();
    RecordStepDataHandler::GetSingleton()->Init();
    ThemeDataHandler::GetSingleton()->Init();
    AllRecordDataHandler::GetSingleton()->Init();

    //资源中心初始化
    AResCenter::GetSingleton()->Init();
    AudioHandler::GetSingleton()->Init();

    //加载 大厅UI
    this->GoToLobby();
}

void AGameLauncher::GoToLobby()
{   
    AKKUIMgr::GetSingleton()->Init();
    AKKUIMgr::GetSingleton()->Show<UMainUIWidget>("MainUICWBP");

    if (mUInitSceneWidget != nullptr)
    {
        mUInitSceneWidget->Hide(true);
        mUInitSceneWidget = nullptr;
    }
}

