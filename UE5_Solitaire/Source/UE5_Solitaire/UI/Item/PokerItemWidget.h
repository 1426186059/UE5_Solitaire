// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKTween/KKTween.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/KKUIMgr.h"

#include "Blueprint/DragDropOperation.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "PaperSprite.h"
#include "UE5_Solitaire/GameLogic/ResCenter.h"

#include "CoreMinimal.h"
#include "PokerItemWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UPokerItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	void OnDrag();
	void OnDragEnd();
public:
	void Init();
	void Show();
	void Hide();
	void Refresh();

	void SetPokerId(int nPokerId);
	void SetTurnOverState(bool bShow, int nStepIndex = -1);
	bool orTurnOverStateIsTrue();
	void ForceShowBackUI();
	void SetEventTriggerState(bool bCanClick);
	void PlayTurnOverAni();
	void DoShakeAni();
	void UpdateVisibaleState();
public:
	int nPokerId;
	bool bTurnOverState = false;
	int32 nStepIndex_ForFirstShowPokerId = -1;
	FVector2D beginPos;
	FVector2D beginScreenSpacePos;
	bool bInDrag = false;
private:
	bool bInit;
	bool bShow;
	bool bCanClick;
	UWidget* tranCardItemAni;
	UImage* mIcon;
	KKTween::Handle mTurnOverTween1;
	KKTween::Handle mTurnOverTween2;
	KKTween::Handle mShakeTween;
};

class FPokerItemDragDropOperation : public FDragDropOperation
{
public:
	UPokerItemWidget* WidgetSource;
	FVector2D BeginScreenSpacePos;
	FVector2D DragStartSlotPos;
	FVector2D DragOffset;
};

UCLASS()
class UPokerItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPokerItemWidget* WidgetSource;
	FVector2D BeginScreenSpacePos;
	FVector2D DragStartSlotPos;
	FVector2D DragOffset;

	UPROPERTY()
	int32 ItemID = 0;

	UPROPERTY()
	int32 Quantity = 0;
};
