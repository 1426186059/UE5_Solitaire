// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ProgressBar.h"
#include "UE5_Solitaire/GameLogic/InitSceneHotUpdateMgr.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UInitSceneWidget.generated.h"


UCLASS()
class UE5_SOLITAIRE_API UInitSceneWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:

public:
	void Init();
	void Show();
	void Hide();
	void Refresh();
	void SetLoadProgress(float Percent01);

public:
	bool bInit;
	UUserWidget* mUIRoot;
	UProgressBar* mUProgressBar;
};
