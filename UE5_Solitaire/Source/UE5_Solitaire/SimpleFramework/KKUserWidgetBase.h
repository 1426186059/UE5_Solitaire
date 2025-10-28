// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/UMGHelper.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "CoreMinimal.h"
#include "KKUserWidgetBase.generated.h"

UCLASS(Abstract)
class UE5_SOLITAIRE_API UKKUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	virtual void Init();
	virtual void Show();
	virtual void Hide(bool bDestroy = false);
	virtual void Refresh();
	virtual void OnLayoutChanged();
	virtual void OnFirstLayoutFinish();
	virtual void CheckFirstLayoutOkToInit();

	bool orFirstLayoutFinish();
	bool orInit();
protected:
	bool bInit;
	bool bFirstLayoutFinish;
	bool bShowUI;
	FVector2D mOldSize;
};
