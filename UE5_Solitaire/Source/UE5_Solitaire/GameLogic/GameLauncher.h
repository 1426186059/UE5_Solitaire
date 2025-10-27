#pragma once

#include "UE5_Solitaire/Test/Test.h"
#include "UE5_Solitaire/UI/UInitSceneWidget.h"
#include "UE5_Solitaire/UI/MainUIWidget.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKEventMgr.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"
#include "UE5_Solitaire/DataTable/DTMgr.h"

#include "InitSceneHotUpdateMgr.h"
#include "ResCenter.h"
#include "AudioHandler.h"
#include "DataCenter.h"
#include "CardHandler.h"
#include "RecordStepDataHandler.h"

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameUserSettings.h"
#include "EngineUtils.h"
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
	void StartEnterGame(void* param);
	void GoToLobby();

private:
	UInitSceneWidget* mUInitSceneWidget;
};