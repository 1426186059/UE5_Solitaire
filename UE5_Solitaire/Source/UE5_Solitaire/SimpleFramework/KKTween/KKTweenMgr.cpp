#include "KKTweenMgr.h"

AKKTweenMgr::AKKTweenMgr()
{
	PrimaryActorTick.bCanEverTick = true;
    this->mManager = KKTweenByList(this);
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
    mManager.Update(DeltaTime);
}

KKTween::KKTweenItem AKKTweenMgr::AddTween(float time, KKTween::Action_Float_Delegate updateFunc = nullptr, KKTween::ActionDelegate finishFunc = nullptr)
{
    return mManager.AddTween(time, updateFunc, finishFunc);
}

KKTween::KKTweenItem AKKTweenMgr::AddTween(UObject* obj, float time, KKTween::Action_Float_Delegate updateFunc = nullptr, KKTween::ActionDelegate finishFunc = nullptr)
{
    return mManager.AddTween(obj, time, updateFunc, finishFunc);
}

KKTween::KKTweenItem AKKTweenMgr::delayedCall(float time, KKTween::ActionDelegate finishFunc = nullptr)
{
    return AddTween(time, nullptr, finishFunc);
}

KKTween::KKTweenItem AKKTweenMgr::delayedCall(UObject* obj, float time, KKTween::ActionDelegate finishFunc = nullptr)
{
    return AddTween(obj, time, nullptr, finishFunc);
}
