#pragma once

#include "DataCenter.h"
#include "InitSceneHotUpdateMgr.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "UE5_Solitaire/UI/UInitSceneWidget.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameUserSettings.h"
#include "GameLauncher.generated.h"

UCLASS()
class UE5_SOLITAIRE_API AGameLauncher : public AKKActorSingleton
{
	GENERATED_BODY()
public:	
	AGameLauncher();

	static AGameLauncher* GetSingleton(bool bCreate = true)
	{
		return AKKActorSingleton::GetSingleton<AGameLauncher>(bCreate);
	}

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	virtual void Tick(float DeltaTime) override;
	void GoToLobby();
};