#include "AudioHandler.h"

void AudioHandler::Init()
{
   
}

UAudioComponent* AudioHandler::GetAudioComponentFromPool()
{
    UAudioComponent* mAudioComponent = nullptr;
    for (auto ac : mAudioComponentPool)
    {
        if (!ac->IsPlaying())
        {
            mAudioComponent = ac;
            break;
        }
    }

    if (mAudioComponent == nullptr)
    {
        mAudioComponent = NewObject<UAudioComponent>(this);
        mAudioComponent->RegisterComponent();
        this->AddInstanceComponent(mAudioComponent);
        this->mAudioComponentPool.Add(mAudioComponent);
    }
    
    mAudioComponent->SetActive(true, true);
    mAudioComponent->SetFloatParameter(FName("Volume"), 1.0f);
    mAudioComponent->SetBoolParameter(TEXT("Loop"), true);
    //mAudioComponent->ClearFlags(RF_PendingKill);
    //mAudioComponent->OnAudioFinished.AddDynamic(this, &AudioHandler::OnPoolSoundPlayFinished);
    return mAudioComponent;
}

UAudioComponent* AudioHandler::OnPoolSoundPlayFinished()
{
    UAudioComponent* AC = mAudioComponentPool.Pop();
    //AC->ClearFlags(RF_PendingKill);
    return AC;
}

void AudioHandler::PlaySound(const FString& name)
{
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

UAudioComponent* AudioHandler::PlaySound2(const FString& name)
{
    FString resPath = FString::Printf(TEXT("/Game/ResourceABs/MainScene/Audio/%s.%s"), *name, *name);
    USoundWave* Sound = LoadObject<USoundWave>(this, *resPath);

    if (!Sound)
    {
        UE_LOG(LogTemp, Error, TEXT("UAudioHandler Error: %s"), *resPath);
        return nullptr;
    }

    if (!Sound->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("UAudioHandler IsValidLowLevel Error: %s"), *resPath);
        return nullptr;
    }

    return nullptr;
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