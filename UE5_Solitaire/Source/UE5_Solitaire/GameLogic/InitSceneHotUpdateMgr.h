// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InitSceneHotUpdateComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InitSceneHotUpdateMgr.generated.h"

UCLASS()
class UE5_SOLITAIRE_API AInitSceneHotUpdateMgr : public AActor
{
	GENERATED_BODY()

public:
	AInitSceneHotUpdateMgr();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	virtual void Tick(float DeltaTime) override;
};
