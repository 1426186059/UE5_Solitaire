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
    mInitSceneHotUpdateComponent = NewObject<UInitSceneHotUpdateComponent>(this, TEXT("InitSceneHotUpdateComponent2"));
    mInitSceneHotUpdateComponent->RegisterComponent();
    this->AddInstanceComponent(mInitSceneHotUpdateComponent);

    UE_LOG(LogTemp, Log, TEXT("UInitSceneHotUpdateComponent Finish Ok 2222"));
    mInitSceneHotUpdateComponent->UpdateProgressFunc = &this->UpdateProgressFunc;
    mInitSceneHotUpdateComponent->UpdateFinishFunc = &this->UpdateFinishFunc;
    mInitSceneHotUpdateComponent->UpdateErrorFunc = &this->UpdateErrorFunc;
    mInitSceneHotUpdateComponent->UpdateVersionFunc = &this->UpdateVersionFunc;
}

void AInitSceneHotUpdateMgr::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AInitSceneHotUpdateMgr::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}