#include "KKTweenMgr.h"
using namespace KKTweenAPI;

AKKTweenMgr::AKKTweenMgr()
{
    PrimaryActorTick.bCanEverTick = true;
    this->mManager = new KKTweenAPI::KKTweenByList(this);
}

void AKKTweenMgr::BeginPlay()
{
    Super::BeginPlay();
}

void AKKTweenMgr::EndPlay(EEndPlayReason::Type Reason)
{
    Super::EndPlay(Reason);
}

void AKKTweenMgr::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    this->Update(DeltaTime);
}

void AKKTweenMgr::Update(float DeltaTime)
{
    mManager->Update(DeltaTime);
}

KKTweenAPI::KKTweenItem* AKKTweenMgr::AddTween(float time, KKTweenAPI::Action_Float_Delegate updateFunc, KKTweenAPI::ActionDelegate finishFunc)
{
    return mManager->AddTween(time, updateFunc, finishFunc);
}

KKTweenAPI::KKTweenItem* AKKTweenMgr::AddTween(UObject* obj, float time, KKTweenAPI::Action_Float_Delegate updateFunc, KKTweenAPI::ActionDelegate finishFunc)
{
    return mManager->AddTween(obj, time, updateFunc, finishFunc);
}

KKTweenAPI::KKTweenItem* AKKTweenMgr::delayedCall(float time, KKTweenAPI::ActionDelegate finishFunc)
{
    return AddTween(time, nullptr, finishFunc);
}

KKTweenAPI::KKTweenItem* AKKTweenMgr::delayedCall(UObject* obj, float time, KKTweenAPI::ActionDelegate finishFunc)
{
    return AddTween(obj, time, nullptr, finishFunc);
}
