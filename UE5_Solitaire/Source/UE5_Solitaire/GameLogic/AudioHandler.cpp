#include "AudioHandler.h"

void AudioHandler::Init()
{
   
}

void AudioHandler::PlaySound(const FString& name)
{
    ///"/Game/ResourceABs/MainScene/Audio/journeynewlevel.journeynewlevel";
    FString resPath = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Audio/%s.%s"), *name, *name);
    USoundWave* Sound = LoadObject<USoundWave>(this, *resPath);
    
    if (!Sound)
    {
        UE_LOG(LogTemp, Error, TEXT("UAudioHandler Error: %s"), *resPath);
        return;
    }

    if (!Sound->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("UAudioHandler IsValidLowLevel Error: %s"), *resPath);
        return;
    }

    UGameplayStatics::PlaySound2D(this, Sound);
}

void StopSound(const FString& name)
{

}

void AudioHandler::PlayBackMusic(const FString& name)
{
    if (mBGMAudioComponent == nullptr)
    {
        // 创建音频组件
        mBGMAudioComponent = NewObject<UAudioComponent>(this);
        mBGMAudioComponent->RegisterComponent(); // 必须注册才能播放
    }

    // 加载 SoundCue
    FString resPath = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Audio/%s.%s"), *name, *name);
    USoundWave* Sound = LoadObject<USoundWave>(this, *resPath);
    
    mBGMAudioComponent->SetSound(Sound);
    mBGMAudioComponent->bAutoActivate = false;
    mBGMAudioComponent->bStopWhenOwnerDestroyed = false;
    mBGMAudioComponent->bShouldRemainActiveIfDropped = true;
    mBGMAudioComponent->SetBoolParameter(TEXT("Loop"), true);
    mBGMAudioComponent->Play(); // 开始播放
}