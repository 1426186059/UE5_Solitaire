#include "KKTweenHead.h"
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

void AKKTweenMgr::SetMaxTweenCount(int nCount)
{
    mManager->SetMaxTweenCount(nCount);
}

KKTweenAPI::KKTweenItem* AKKTweenMgr::AddTween(UObject* obj, float time, KKTweenAPI::Action_Float_Delegate updateFunc, KKTweenAPI::ActionDelegate finishFunc)
{
    if (obj == nullptr)
    {
        obj = this;
    }

    return mManager->AddTween(obj, time, updateFunc, finishFunc);
}

KKTweenAPI::KKTweenItem* AKKTweenMgr::delayedCall(UObject* obj, float time, KKTweenAPI::ActionDelegate finishFunc)
{
    return AddTween(obj, time, nullptr, finishFunc);
}


