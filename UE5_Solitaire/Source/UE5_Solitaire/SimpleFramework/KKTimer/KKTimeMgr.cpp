#include "KKTimeMgr.h"

AKKTimeMgr::AKKTimeMgr()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKKTimeMgr::BeginPlay()
{
	Super::BeginPlay();
}

void AKKTimeMgr::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AKKTimeMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->Update(DeltaTime);
}