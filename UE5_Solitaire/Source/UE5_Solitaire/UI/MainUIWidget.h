// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/UMGAdapterTool.h"
#include "UE5_Solitaire/GameLogic/GameConst.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

#include "Item/PokerItemWidget.h"

#include "CoreMinimal.h"
#include "MainUIWidget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void Init();
	void Show();
	void Hide();
	void Refresh();
	void OnScreenSizeChanged();

	UFUNCTION() void OnBtnClicked_GameNodeBtn();
	
	void InitGame();
	void RecoverGame(bool bPlayAni);
	void RecycleAndInitCardGo();

	FVector2D GetRelativePosByGo(UWidget* target);
	void SetRelativePos(UWidget* target, FVector2D relativePos);
	float GetTop7_Gap_Height(int nTopIndex);
	FVector2D GetCardNodeTop7MaxHeightPos(int nTopIndex);
	FVector2D GetCardNodeTop7NextMaxHeightPos(int nTopIndex);
	FVector2D GetCardNodeTop7Pos(int nTopIndex, int nHeightIndex);
	FVector2D GetCardNode4APos(int nTopIndex);
	FVector2D GetCardNodeDraw3Pos(int nIndex);
	FVector2D GetCardNodeSendPokerPos();
private:
	bool bInit;
	UUserWidget* mUIRoot;
	FVector2D mOldSize;

	UCanvasPanel* PokerItemParent;
	UWidget* mFaPaiPos;
	TArray<UWidget*> tableCardNode4APos;
	TArray<UWidget*> tableCardNodeTop7Pos;

	TArray<UPokerItemWidget*> tableCardNode4AGo;
	TArray<UPokerItemWidget*> tableCardNodeTop7Go;
	TArray<UPokerItemWidget*> mSendCardListGo;

	TArray<int> mInitSendCardList;

	static const float N_TOP7_GAP_HEIGHT = 50.0;
	SolitaireGameMode nGameMode = SolitaireGameMode::Normal;
	bool bGameEnd = false;
	float fRobotThinkingTime = 0;
	float fIQTime = 0;
};
