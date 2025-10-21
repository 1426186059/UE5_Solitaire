#include "InitSceneHotUpdateMgr.h"

AInitSceneHotUpdateMgr::AInitSceneHotUpdateMgr()
{
    PrimaryActorTick.bCanEverTick = true;

    //增加组件方法1
    //mUInitSceneHotUpdateMgr = CreateDefaultSubobject<UInitSceneHotUpdateComponent>(TEXT("InitSceneHotUpdateComponent"));
    //this->AddInstanceComponent(mUInitSceneHotUpdateMgr);
}

void AInitSceneHotUpdateMgr::BeginPlay()
{
    Super::BeginPlay();

    //增加组件方法2
    mUInitSceneHotUpdateMgr = NewObject<UInitSceneHotUpdateComponent>(this, TEXT("InitSceneHotUpdateComponent2"));
    mUInitSceneHotUpdateMgr->RegisterComponent();
    this->AddInstanceComponent(mUInitSceneHotUpdateMgr);

    mUInitSceneHotUpdateMgr->UpdateProgressFunc = this->UpdateProgressFunc;
    mUInitSceneHotUpdateMgr->UpdateFinishFunc = this->UpdateFinishFunc;
    mUInitSceneHotUpdateMgr->UpdateErrorFunc = this->UpdateErrorFunc;
    mUInitSceneHotUpdateMgr->UpdateVersionFunc = this->UpdateVersionFunc;
}

void AInitSceneHotUpdateMgr::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AInitSceneHotUpdateMgr::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}