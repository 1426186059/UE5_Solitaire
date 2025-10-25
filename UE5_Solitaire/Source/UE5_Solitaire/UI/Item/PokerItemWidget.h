// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

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
private:
	bool bInit;
	UUserWidget* mUIRoot;
};
