// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Widget.h"
#include "Components/Image.h"

#include "KKSingleton.h"
#include "UMGHelper.h"
#include "UEHelper.h"

class UMGAdapterTool : public KKSingleton<UMGAdapterTool>
{
public:
	void Init()
	{
		if (bInit) return;
		bInit = true;
	}

	void FitBG(UImage* mUImage) //这个的话，根据原有精灵尺寸，等比例缩放
	{
		FIntPoint mResolution = UEHelper::GetScreenResolution();
		FIntPoint mSize = UMGHelper::GetImageOriginalSize(mUImage);

		float ratio1 = mResolution.Y / (float)mResolution.X;
		float ratio2 = mSize.Y / (float)mSize.X;

		if (ratio1 > ratio2) //屏幕更窄
		{
			float fWidth = mResolution.Y / mSize.Y * mSize.X;
			UMGHelper::SetWidgetSize(mUImage, FVector2D(fWidth, mResolution.Y));
		}
		else if(ratio1 < ratio2) //屏幕更宽
		{
			float fHeight = mResolution.X / mSize.X * mSize.Y;
			UMGHelper::SetWidgetSize(mUImage, FVector2D(mResolution.X, fHeight));
		}

		UE_LOG(LogTemp, Log, TEXT("UMGAdapterTool mResolution: %s"), *mResolution.ToString());
		UE_LOG(LogTemp, Log, TEXT("UMGAdapterTool mSize: %s"), *mSize.ToString());
		UE_LOG(LogTemp, Log, TEXT("UMGAdapterTool FitBG: %.3f, %.3f"), ratio1, ratio2);
	}

	void FitBG(UWidget* mWidget)
	{
		FIntPoint mResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
		FVector2D mBGXY = mWidget->GetDesiredSize();   // FVector2D
		
		float ratio1 = mResolution.Y / (float)mResolution.X;
		float ratio2 = mBGXY.Y / (float)mBGXY.X;

		if (ratio1 > ratio2) //屏幕更长了
		{
			float scale = ratio1 / ratio2;
			UMGHelper::SetWidgetScale(mWidget, FVector2D(scale, scale));
		}
	}
	
private:
	bool bInit;
};
