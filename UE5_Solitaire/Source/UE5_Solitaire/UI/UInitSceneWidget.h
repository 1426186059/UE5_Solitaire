// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ProgressBar.h"
#include "UE5_Solitaire/GameLogic/InitSceneHotUpdateMgr.h"
#include "UE5_Solitaire/GameLogic/GameConst.h"
#include "UE5_Solitaire/SimpleFramework/KKEventMgr.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UInitSceneWidget.generated.h"


UCLASS()
class UE5_SOLITAIRE_API UInitSceneWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void Init();
	void Show();
	void Hide(bool bDestory = false);
	void Refresh();

	void UpdateProgressFunc(float Percent01);
	void UpdateFinishFunc();
	void UpdateErrorFunc();
	void UpdateVersionFunc();

public:
	bool bInit;
	UUserWidget* mUIRoot;
	UProgressBar* mUProgressBar;

	AInitSceneHotUpdateMgr* mInitSceneHotUpdateMgr;
};
