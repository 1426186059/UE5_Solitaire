// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "UE5_Solitaire/GameLogic/GameLogic.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLoseWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UGameLoseWidget : public UWUIBase
{
	GENERATED_BODY()
public:
	static FString GetMyUIPath()
	{
		return TEXT("GameLoseCWBP");
	}

	virtual void Init() override;
	virtual void OnShow() override;
	virtual void Refresh() override;

	UFUNCTION() void OnBtnClicked_Cancel();
	UFUNCTION() void OnBtnClicked_NewGame();
public:
	bool bInit;
};
