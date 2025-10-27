#include "AudioHandler.h"

void UAudioHandler::Init()
{
   
}

void UAudioHandler::PlaySound(FName name)
{
    FString resPath = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Audio/%s.%s"), *name.ToString(), *name.ToString());
    USoundWave* Sound = LoadObject<USoundWave>(nullptr, *resPath);
    UGameplayStatics::PlaySound2D(this, Sound);
}

void UAudioHandler::PlayBackMusic(FName name)
{
    if (mBGMAudioComponent == nullptr)
    {
        // 创建音频组件
        mBGMAudioComponent = NewObject<UAudioComponent>(this);
        mBGMAudioComponent->RegisterComponent(); // 必须注册才能播放
    }

    // 加载 SoundCue
    FString resPath = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Audio/%s.%s"), *name.ToString(), *name.ToString());
    USoundWave* Sound = LoadObject<USoundWave>(nullptr, *resPath);
    
    mBGMAudioComponent->SetSound(Sound);
    mBGMAudioComponent->bAutoActivate = false;
    mBGMAudioComponent->bStopWhenOwnerDestroyed = false;
    mBGMAudioComponent->bShouldRemainActiveIfDropped = true;
    mBGMAudioComponent->SetBoolParameter(TEXT("Loop"), true);
    mBGMAudioComponent->Play(); // 开始播放
}