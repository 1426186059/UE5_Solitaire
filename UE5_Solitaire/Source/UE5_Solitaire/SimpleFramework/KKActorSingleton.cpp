#include "KKActorSingleton.h"

TMap<TSubclassOf<AActor>, TWeakObjectPtr<AActor>> AKKActorSingleton::mInstanceDic = {};

void AKKActorSingleton::BeginPlay()
{
	Super::BeginPlay();
	AddSingleton();
}

void AKKActorSingleton::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	RemoveSingleton();
}