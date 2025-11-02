// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKTween/KKTween.h"
#include "UE5_Solitaire/SimpleFramework/KKTimer/KKTimer.h"
#include "UE5_Solitaire/SimpleFramework/KKPool/KKPool.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/KKUIMgr.h"

#include "UE5_Solitaire/GameLogic/GameConst.h"
#include "UE5_Solitaire/GameLogic/DataCenter.h"
#include "UE5_Solitaire/GameLogic/ResCenter.h"
#include "UE5_Solitaire/GameLogic/RecordStepDataHandler.h"
#include "UE5_Solitaire/GameLogic/CardHandler.h"
#include "UE5_Solitaire/GameLogic/AudioHandler.h"
#include "UE5_Solitaire/GameLogic/ThemeDataHandler.h"
#include "UE5_Solitaire/GameLogic/AllRecordDataHandler.h"
#include "UE5_Solitaire/GameLogic/StatisticDataHandler.h"

#include "GameWinAniMgr.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

class UPokerItemWidget;

#include "CoreMinimal.h"
#include "MainUIWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UMainUIWidget : public UWUIBase
{
	GENERATED_BODY()
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& InMouseEvent) override;
public:
	virtual void OnCreate() override;
	virtual void Init() override;
	virtual void OnLayoutChanged() override;
	virtual void CheckFirstLayoutOkToShow() override;

public:

	UFUNCTION() void OnBtnClicked_GameNodeBtn();
	UFUNCTION() void OnBtnClicked_SendPokerBtn();
	UFUNCTION() void OnBtnClicked_BGBtn();

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
	void UpdateGameMode();

	std::tuple<bool, UPokerItemWidget*> orThisStepTurnOverPokerIsTrue(int nStepIndex);
	int GetTop7HideCardCount();
	void onAddScore_InitParam();
	void onAddScore();
	bool JudgeWin();

	bool orPosTypeInfoEqual(TArray<int32> info1, TArray<int32> info2);
	bool onIsLoopOp();
	void onAddIQ();

	void UpdateLeftHandState();
	bool orHavePopView();
	void UpdateGameTimeState(bool bPause);
	void Set_FastGame();
	void Set_AutoHinted();
	void TimerPerSecondUpdate();
	void TellRobot_PlayerAlive();
	void OnClickChuPai();
	void RefreshDrawZone();
	void OnClickToMovePokerPos(UPokerItemWidget* mCardItem);
	void OnDragBegin(UPokerItemWidget* mDragCardItem);
	void OnDrag(UPokerItemWidget* mDragCardItem);
	void OnDragEndToMovePokerPos(UPokerItemWidget* mCardItem);
	void LockTargetToMove(UPokerItemWidget* mCardItem, int nPosType, int nIndex);

	FVector2D GetPosByCardItem(UPokerItemWidget* mCardItem);
	FVector2D GetPosByPosTypeInfo(const TArray<int>& nPosTypeInfo);
	TArray<UPokerItemWidget*> GetSelectCardItemList(UPokerItemWidget* mCardItem);
	TArray<int> GetPokerPosType(UPokerItemWidget* mDragCardItem);

	TArray<UPokerItemWidget*> RemoveArrayFromTop7Go(int nTopIndex, int nRemoveIndex);
	void InsertArrayToTop7Go(int nTopIndex, const TArray<UPokerItemWidget*>& tableArray);
	TArray<UPokerItemWidget*> SetDragEndRemoveSelfFromArray(UPokerItemWidget* mDragCardItem);
	void UpdateAllPokerEventTriggerState();

	bool orCanIn4A(int n4AIndex, UPokerItemWidget* mCardItem);
	bool orCanInNode7(int n7Index, UPokerItemWidget* mCardItem);

	void DoActionEnd();
	void CheckGameEnd();
	void DoA4AllDataOp(TFunction<void()> finishFunc = nullptr);
	void PlayWinAni();
	void AutoShouPai(TFunction<void()> finishFunc = nullptr);
	void DoWinAnimation();
	void ShowGameEndView();
	TArray<FVector2D> GetTableA4WorldPos();
	TArray<int32> GetTableA4Color();

	void OnClickDraw3Move(UPokerItemWidget* mCardItem, FVector2D fromPos, FVector2D toPos, bool bUndo, TFunction<void()> finishFunc = nullptr);
	void OnDragEndMove(UPokerItemWidget* mCardItem, FVector2D fromPos, FVector2D toPos, bool bUndo, TFunction<void()> finishFunc);
	void DoTop7ReSizeHeightAni(int nTop7Index);

	void DoWhenSet_FastGame();
	void PlayToA4EffectAni(int32 nIndex, UPokerItemWidget* mCardItem);
	void OnFastGameToResultA4();
private:
	UUserWidget* mUIRoot;
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
	TArray<UPokerItemWidget*> tableFinalA4AniCardItem;

	const float N_TOP7_GAP_HEIGHT = 50.0f;
	int32 nGameMode;
	bool bGameEnd = false;
	bool bInitA4Effect;
	float fRobotThinkingTime = 0;
	float fIQTime = 0;
	int nGetScore_nLastTop7HideCardCount = 0;
	int nRemainHintCount_InDraw3AndSendCardList = 0;
	TMap<UPokerItemWidget*, KKTween::Handle> mapCardItemTween;
	KKTween::Handle tween_OnFastGameToResultA4;

	UImage* mBG;

	GameWinAniMgr* GameWinAniMgr;
	FTimerHandle mTimer;
};
