// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"
#include "DataCenter.h"

#include "AudioHandler.generated.h"

UCLASS()
class AudioHandler : public AKKActorSingleton
{
	GENERATED_BODY()
public:
	static AudioHandler* GetSingleton(bool bCreate = true)
	{
		return AKKActorSingleton::GetActorSingleton<AudioHandler>(bCreate);
	}

public:
	void Init();
	void PlaySound(const FString& name);
	UAudioComponent* PlaySound2(const FString& name);
	void StopSound(UAudioComponent* mAudio);
	void PlayBackMusic(const FString& name);
private:
	UAudioComponent* OnPoolSoundPlayFinished();
	UAudioComponent* GetAudioComponentFromPool();

private:
	UAudioComponent* mBGMAudioComponent;
	TArray<UAudioComponent*> mAudioComponentPool;
};
