// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InitSceneHotUpdateComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5_SOLITAIRE_API UInitSceneHotUpdateComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UInitSceneHotUpdateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void RequestAllPrimaryAsset();
	void OnAssetLoadCompleted();
	void RequestLoadAllRes();


	TSharedPtr<FStreamableHandle> mFStreamableHandle;
};
