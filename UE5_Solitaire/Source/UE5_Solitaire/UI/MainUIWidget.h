// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/KKTween/KKTween.h"
#include "UE5_Solitaire/SimpleFramework/KKTimer/KKTimer.h"
#include "UE5_Solitaire/SimpleFramework/KKPool/KKPool.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/KKUI.h"

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
class UPokerAnimationItemW;
class TopBottomView;

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
	UFUNCTION() void OnBtnClicked_SendPokerBtn();
	UFUNCTION() void OnBtnClicked_BGBtn();

	UFUNCTION() void OnBtnClicked_GameNodeBtn();
	UFUNCTION() void OnBtnClicked_UndoBtn();
	UFUNCTION() void OnBtnClicked_ForwardBtn();
	UFUNCTION() void OnBtnClicked_ThemeBtn();
	UFUNCTION() void OnBtnClicked_SettingBtn();

	void InitGame();
	void RecoverGame(bool bPlayAni = false);
	void RecycleAndInitCardGo();
	void NewGameBegin(bool bRePlay = false);
	
	float GetTop7_Gap_Height(int32 nTopIndex);
	FVector2D GetCardNodeTop7MaxHeightPos(int32 nTopIndex);
	FVector2D GetCardNodeTop7NextMaxHeightPos(int32 nTopIndex);
	FVector2D GetCardNodeTop7Pos(int32 nTopIndex, int32 nHeightIndex);
	FVector2D GetCardNode4APos(int32 nTopIndex);
	FVector2D GetCardNodeDraw3Pos(int32 nIndex);
	FVector2D GetCardNodeSendPokerPos();

	void NewGameBegin_ForNormal(bool bForceNewGame);
	void UpdateGameMode();

	std::tuple<bool, UPokerItemWidget*> orThisStepTurnOverPokerIsTrue(int32 nStepIndex);
	int32 GetTop7HideCardCount();
	void onAddScore_InitParam();
	void onAddScore();
	bool JudgeWin();

	bool orPosTypeInfoEqual(const TArray<int32>& info1, const TArray<int32>& info2);
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
	void LockTargetToMove(UPokerItemWidget* mCardItem, int32 nPosType, int32 nIndex);

	FVector2D GetPosByCardItem(UPokerItemWidget* mCardItem);
	FVector2D GetPosByPosTypeInfo(const TArray<int32>& nPosTypeInfo);
	TArray<UPokerItemWidget*> GetSelectCardItemList(UPokerItemWidget* mCardItem);
	TArray<int32> GetPokerPosType(UPokerItemWidget* mDragCardItem);

	TArray<UPokerItemWidget*> RemoveArrayFromTop7Go(int32 nTopIndex, int32 nRemoveIndex);
	void InsertArrayToTop7Go(int32 nTopIndex, const TArray<UPokerItemWidget*>& tableArray);
	TArray<UPokerItemWidget*> SetDragEndRemoveSelfFromArray(UPokerItemWidget* mDragCardItem);
	void UpdateAllPokerEventTriggerState();

	bool orCanIn4A(int32 n4AIndex, UPokerItemWidget* mCardItem);
	bool orCanInNode7(int32 n7Index, UPokerItemWidget* mCardItem);

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
	void OnDragEndMove(UPokerItemWidget* mCardItem, FVector2D fromPos, FVector2D toPos, bool bUndo, TFunction<void()> finishFunc = nullptr);
	void DoTop7ReSizeHeightAni(int32 nTop7Index);

	void DoWhenSet_FastGame();
	void PlayToA4EffectAni(int32 nIndex, UPokerItemWidget* mCardItem);
	void OnFastGameToResultA4();

	void RefreshAllPokerState();
	int32 GetNowRecordStepIndex();
	void PlayRecordUndoAni();
	void PlayRecordForwardAni();

public:
	UUserWidget* mUIRoot;
	UCanvasPanel* PokerItemParent;
	UCanvasPanel* GameWinAniParent;
	FVector2D tranFaPaiPos;
	FVector2D mCardNodeDraw3BeginPos;
	TArray<FVector2D> tableCardNode4APos;
	TArray<FVector2D> tableCardNodeTop7Pos;

	TArray<TArray<UPokerItemWidget*>> tableCardNode4AGo;
	TArray<TArray<UPokerItemWidget*>> tableCardNodeTop7Go;
	TArray<UPokerItemWidget*> mSendCardListGo;
	TArray<UPokerItemWidget*> tableCardDraw3Go;

	TArray<int32> mLastSendCardList;
	TArray<UPokerItemWidget*> tableFinalA4AniCardItem;

	const float N_TOP7_GAP_HEIGHT = 50.0f;
	int32 nGameMode;
	bool bGameEnd = false;
	bool bInitA4Effect;
	float fRobotThinkingTime = 0;
	float fIQTime = 0;
	int32 nGetScore_nLastTop7HideCardCount = 0;
	int32 nRemainHintCount_InDraw3AndSendCardList = 0;
	TMap<UPokerItemWidget*, KKTween::Handle> mapCardItemTween;
	KKTween::Handle tween_OnFastGameToResultA4;

	UImage* mBG;
	UPROPERTY() UGameWinAniMgr* GameWinAniMgr;
	FTimerHandle mTimer;

	TSharedPtr<TopBottomView> mTopBottomView;
};
