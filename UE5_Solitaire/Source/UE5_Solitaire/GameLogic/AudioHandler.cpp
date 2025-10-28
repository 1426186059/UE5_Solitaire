#include "AudioHandler.h"

void UAudioHandler::Init()
{
   
}

void UAudioHandler::PlaySound(const FString& name)
{
    ///"/Game/ResourceABs/MainScene/Audio/journeynewlevel.journeynewlevel";
    FString resPath = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Audio/%s.%s"), *name, *name);
    USoundBase* Sound = LoadObject<USoundBase>(nullptr, *resPath);

    UE_LOG(LogTemp, Error, TEXT("UAudioHandler Error: %s"), *resPath);
    if (!Sound)
    {
        UE_LOG(LogTemp, Error, TEXT("UAudioHandler Error: %s"), *resPath);
        return;
    }

    UGameplayStatics::PlaySound2D(this, Sound);
}

void UAudioHandler::PlayBackMusic(const FString& name)
{
    if (mBGMAudioComponent == nullptr)
    {
        // 创建音频组件
        mBGMAudioComponent = NewObject<UAudioComponent>(this);
        mBGMAudioComponent->RegisterComponent(); // 必须注册才能播放
    }

    // 加载 SoundCue
    FString resPath = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Audio/%s.%s"), *name, *name);
    USoundWave* Sound = LoadObject<USoundWave>(nullptr, *resPath);
    
    mBGMAudioComponent->SetSound(Sound);
    mBGMAudioComponent->bAutoActivate = false;
    mBGMAudioComponent->bStopWhenOwnerDestroyed = false;
    mBGMAudioComponent->bShouldRemainActiveIfDropped = true;
    mBGMAudioComponent->SetBoolParameter(TEXT("Loop"), true);
    mBGMAudioComponent->Play(); // 开始播放
}