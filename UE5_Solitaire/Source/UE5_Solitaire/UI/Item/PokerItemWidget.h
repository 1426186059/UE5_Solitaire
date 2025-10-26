// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	void SetTurnOverState(bool bShow, int nStepIndex = 0);
	bool orTurnOverStateIsTrue();
	void ForceShowBackUI();
	void SetEventTriggerState(bool bCanClick);
	
private:
	bool bInit;

	UUserWidget* mUIRoot;
	UImage* mIcon;

	int nPokerId;
	bool bTurnOverState = false;
	int32 nStepIndex_ForFirstShowPokerId = -1;
	FVector2D beginPos = {};
	bool bInDrag = false;
};
