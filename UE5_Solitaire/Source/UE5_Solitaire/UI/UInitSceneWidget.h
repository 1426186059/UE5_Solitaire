// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ProgressBar.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UInitSceneWidget.generated.h"


UCLASS()
class UE5_SOLITAIRE_API UInitSceneWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void SetLoadProgress(float Percent01);
};
