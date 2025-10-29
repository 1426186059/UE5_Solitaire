#include "KKActorSingleton.h"

void AKKActorSingleton::BeginPlay()
{
	Super::BeginPlay();
	if (mInstance == nullptr)
	{
		mInstance = this;
	}
	else if (mInstance != this)
	{
		UE_LOG(LogTemp, Error, TEXT("KKActorSingleton Error"));
	}
}

void AKKActorSingleton::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	mInstance = nullptr;
}