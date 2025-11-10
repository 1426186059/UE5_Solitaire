// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMGHelper.h"
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UWUIBase.generated.h"

UCLASS(Abstract)
class UE5_SOLITAIRE_API UWUIBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	virtual FString GetMyResPath();
	virtual void OnCreate();
	virtual void OnLayoutChanged();
	virtual void OnShow();

	virtual void Init();
	virtual void Show();
	virtual void Hide(bool bDestroy = false);
	virtual void Refresh();
	virtual void CheckFirstLayoutOkToShow();

	bool orFirstLayoutFinish();
	bool orInit();
protected:
	bool bInit;
	bool bCanClickUI;
	bool bFirstLayoutFinish;
	bool bShowUI;
	FVector2D mOldSize;
};
