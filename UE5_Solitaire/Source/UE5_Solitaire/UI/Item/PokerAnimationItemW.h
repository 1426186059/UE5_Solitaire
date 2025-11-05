// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKTween/KKTween.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/KKUIMgr.h"
#include "UE5_Solitaire/GameLogic/ResCenter.h"

#include "CoreMinimal.h"
#include "PokerAnimationItemW.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UPokerAnimationItemW : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	void InitPokerId(int poker_id);
	void Show();
	void Hide();
	void Refresh();
	void SetPokerId(int poker_id);
public:
	int nPokerId;
	bool bTurnOverState = false;
	int32 nStepIndex_ForFirstShowPokerId = -1;
	FVector2D beginPos = {};
	bool bInDrag = false;
private:
	bool bInit;
};
