#include "InitSceneHotUpdateMgr.h"

AInitSceneHotUpdateMgr::AInitSceneHotUpdateMgr()
{
    PrimaryActorTick.bCanEverTick = true;

    //挂载 热更组件, 如果不在这里挂载，会导致游戏崩溃
    UInitSceneHotUpdateComponent* mUInitSceneHotUpdateMgr = CreateDefaultSubobject<UInitSceneHotUpdateComponent>(TEXT("InitSceneHotUpdateComponent"));
    this->AddInstanceComponent(mUInitSceneHotUpdateMgr);
    mUInitSceneHotUpdateMgr->RegisterComponent();
}

void AInitSceneHotUpdateMgr::BeginPlay()
{
    Super::BeginPlay();
}

void AInitSceneHotUpdateMgr::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AInitSceneHotUpdateMgr::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}