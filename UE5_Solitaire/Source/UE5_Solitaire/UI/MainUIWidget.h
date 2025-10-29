// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/UMGAdapterTool.h"
#include "UE5_Solitaire/SimpleFramework/KKUserWidgetBase.h"
#include "UE5_Solitaire/SimpleFramework/KKTween/KKTween.h"

#include "UE5_Solitaire/GameLogic/GameConst.h"
#include "UE5_Solitaire/GameLogic/DataCenter.h"
#include "UE5_Solitaire/GameLogic/ResCenter.h"
#include "UE5_Solitaire/GameLogic/RecordStepDataHandler.h"
#include "UE5_Solitaire/GameLogic/CardHandler.h"
#include "UE5_Solitaire/GameLogic/AudioHandler.h"
#include "UE5_Solitaire/GameLogic/ThemeDataHandler.h"
#include "UE5_Solitaire/GameLogic/AllRecordDataHandler.h"
#include "UE5_Solitaire/GameLogic/StatisticDataHandler.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

#include "Item/PokerItemWidget.h"

#include "CoreMinimal.h"
#include "MainUIWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UMainUIWidget : public UKKUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	virtual void Init() override;
	virtual void OnLayoutChanged() override;
	virtual void CheckFirstLayoutOkToInit() override;

	UFUNCTION() void OnBtnClicked_GameNodeBtn();
	
	void InitGame();
	void RecoverGame(bool bPlayAni = false);
	void RecycleAndInitCardGo();
	void NewGameBegin(bool bRePlay = false);
	
	float GetTop7_Gap_Height(int nTopIndex);
	FVector2D GetCardNodeTop7MaxHeightPos(int nTopIndex);
	FVector2D GetCardNodeTop7NextMaxHeightPos(int nTopIndex);
	FVector2D GetCardNodeTop7Pos(int nTopIndex, int nHeightIndex);
	FVector2D GetCardNode4APos(int nTopIndex);
	FVector2D GetCardNodeDraw3Pos(int nIndex);
	FVector2D GetCardNodeSendPokerPos();

	void NewGameBegin_ForNormal(bool bForceNewGame);
private:
	UUserWidget* mUIRoot;
	FVector2D mOldSize;
	UCanvasPanel* PokerItemParent;
	FVector2D tranFaPaiPos;
	FVector2D mCardNodeDraw3BeginPos;
	TArray<FVector2D> tableCardNode4APos;
	TArray<FVector2D> tableCardNodeTop7Pos;

	TArray<TArray<UPokerItemWidget*>> tableCardNode4AGo;
	TArray<TArray<UPokerItemWidget*>> tableCardNodeTop7Go;
	TArray<UPokerItemWidget*> mSendCardListGo;
	TArray<UPokerItemWidget*> tableCardDraw3Go;

	TArray<int> mLastSendCardList;

	const float N_TOP7_GAP_HEIGHT = 50.0f;
	int32 nGameMode;
	bool bGameEnd = false;
	float fRobotThinkingTime = 0;
	float fIQTime = 0;
};
