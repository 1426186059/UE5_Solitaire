// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "UE5_Solitaire/GameLogic/GameLogic.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWinWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UGameWinWidget : public UWUIBase
{
	GENERATED_BODY()

public:
	static FString GetMyUIPath()
	{
		return TEXT("GameWinCWBP");
	}

	virtual void Init() override;
	virtual void OnShow() override;
	virtual void Refresh() override;
	virtual void HideWithAni(bool bDestroy = false) override;
	UFUNCTION() void OnBtnClicked();
};
