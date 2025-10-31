#include "KKFrameUpdateMgr.h"

AKKFrameUpdateMgr::AKKTimeMgr()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKKFrameUpdateMgr::BeginPlay()
{
	Super::BeginPlay();
}

void AKKFrameUpdateMgr::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AKKFrameUpdateMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->Update(DeltaTime);
}