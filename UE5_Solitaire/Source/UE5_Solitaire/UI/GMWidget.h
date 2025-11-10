// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"

#include "CoreMinimal.h"
#include "GMWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UGMWidget : public UWUIBase
{
	GENERATED_BODY()
public:
	static FString GetMyUIPath()
	{
		return TEXT("GMCWBP");
	}
	virtual void OnCreate() override;
	virtual void Init() override;
	virtual void Show() override;
	virtual void Hide(bool bDestory = false) override;
	virtual void Refresh() override;
	virtual void CheckFirstLayoutOkToShow() override;

	UFUNCTION() void OnBtnClicked_AutoShouPaiBtn();
	UFUNCTION() void OnBtnClicked_WinAnimationBtn();

private:
	//UButton* StageRewardBtn;
	//UButton* WinAnimationBtn;
	//UButton* GuideFastPlayBtn;
};
