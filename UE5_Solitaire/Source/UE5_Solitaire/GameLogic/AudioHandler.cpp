#include "AudioHandler.h"

void UAudioHandler::Init()
{
    USoundWave* SoundWaveAsset = nullptr;
    // 1. 简单播放（2D）
    UGameplayStatics::PlaySound2D(this, SoundWaveAsset);

    // 2. 3D 播放（带位置）
    //UGameplayStatics::PlaySoundAtLocation(this, SoundWave, FVector(0, 0, 0));

    // 3. 使用 Audio Component（可控制）
    //UAudioComponent* AC = UGameplayStatics::SpawnSoundAtLocation(this, Sound, Location);
    //if (AC)
    //{
    //    AC->SetVolumeMultiplier(0.8f);
    //    AC->SetPitchMultiplier(1.2f);
    //    AC->OnAudioFinished.AddLambda([]() {
    //        UE_LOG(LogTemp, Log, TEXT("Sound finished!"));
    //        });
    //}
}
