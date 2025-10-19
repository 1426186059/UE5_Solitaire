// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataCenter.h"
#include "InitSceneHotUpdateMgr.h"
#include "UE5_Solitaire/UI/UInitSceneWidget.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameUserSettings.h"
#include "GameLauncher.generated.h"

UCLASS()
class UE5_SOLITAIRE_API AGameLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
