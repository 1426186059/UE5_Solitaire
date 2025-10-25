// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE5_Solitaire/SimpleFramework/UMGAdapterTool.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

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
private:
	bool bInit;
	UUserWidget* mUIRoot;
	FVector2D mOldSize;
	TArray<UUserWidget*> mSendCardListGo;
};
