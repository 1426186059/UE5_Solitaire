// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLauncher.h"

AGameLauncher::AGameLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGameLauncher::BeginPlay()
{
	Super::BeginPlay();

    //设置目标帧率
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        UserSettings->SetFrameRateLimit(60);
        UserSettings->ApplySettings(false);
        UserSettings->SaveSettings();
    }

    DataCenter::GetInstance()->Init();
}

void AGameLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGameLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

