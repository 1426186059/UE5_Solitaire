// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "UE5_Solitaire/GameLogic/GameLogic.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameBeginWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UGameBeginWidget : public UWUIBase
{
	GENERATED_BODY()

public:
	static FString GetMyUIPath()
	{
		return TEXT("/Game/ResourceABs/MainScene/BPS/UI/GameBeginCWBP.GameBeginCWBP");
	}

	virtual void OnCreate() override;
	virtual void Init() override;
	virtual void OnShow() override;
	virtual void Refresh() override;

	UFUNCTION() void OnBtnClicked_Hide();
	UFUNCTION() void OnBtnClicked_ReStart();
	UFUNCTION() void OnBtnClicked_NewGame();
	UFUNCTION() void OnBtnClicked_Draw1();
	UFUNCTION() void OnBtnClicked_Draw2();
	UFUNCTION() void OnBtnClicked_Draw3();

	void UpdateSwitchDraw3State();

};
