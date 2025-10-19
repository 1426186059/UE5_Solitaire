// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InitSceneHotUpdateMgr.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UInitSceneHotUpdateMgr : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UInitSceneHotUpdateMgr();

protected:
	virtual void BeginPlay() override;

public:	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void LoadUIAsset(FPrimaryAssetId AssetId);
	void OnAssetLoadCompleted();
	void RequestLoadAllRes();

	// 头文件里存句柄
	TSharedPtr<FStreamableHandle> mFStreamableHandle;
};
