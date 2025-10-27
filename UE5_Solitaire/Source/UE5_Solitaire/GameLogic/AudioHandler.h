// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#include "UE5_Solitaire/SimpleFramework/KKObjectSingleton.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"
#include "DataCenter.h"

#include "AudioHandler.generated.h"

UCLASS()
class UAudioHandler : public UKKObjectSingleton
{
	GENERATED_BODY()
public:
	static UAudioHandler* GetSingleton(bool bCreate = true)
	{
		return UKKObjectSingleton::GetSingleton<UAudioHandler>(bCreate);
	}

public:
	void Init();
	void PlaySound(FName name);
	void PlayBackMusic(FName name);

private:
	UAudioComponent* mBGMAudioComponent;
};
