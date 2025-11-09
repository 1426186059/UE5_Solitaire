// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLoseWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UGameLoseWidget : public UWUIBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void CheckFirstLayoutOkToShow() override;
public:
	bool bInit;
};
