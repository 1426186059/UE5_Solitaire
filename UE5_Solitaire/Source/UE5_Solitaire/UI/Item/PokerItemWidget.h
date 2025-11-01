// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKTween/KKTween.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/KKUIMgr.h"

#include "Blueprint/UserWidget.h"
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

public:
	int nPokerId;
	bool bTurnOverState = false;
	int32 nStepIndex_ForFirstShowPokerId = -1;
	FVector2D beginPos = {};
	bool bInDrag = false;
private:
	bool bInit;
	UWidget* tranCardItemAni;
	UImage* mIcon;
	KKTweenItem::Handle mTurnOverTween1;
	KKTweenItem::Handle mTurnOverTween2;
	KKTweenItem::Handle mShakeTween;
};
