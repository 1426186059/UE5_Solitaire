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

	void FitBG(UUserWidget* mRootWidget, UImage* mUImage) //这个的话，根据原有精灵尺寸，等比例缩放
	{
		FVector2D mScreenSize = UMGHelper::GetUMGRootSzie(mRootWidget);
		FIntPoint mSize = UMGHelper::GetImageOriginalSize(mUImage);

		float ratio1 = mScreenSize.Y / (float)mScreenSize.X;
		float ratio2 = mSize.Y / (float)mSize.X;


		FVector2D mActualSize;
		if (ratio1 > ratio2) //屏幕更窄
		{
			float fWidth = mScreenSize.Y * mSize.X / (float)mSize.Y;
			mActualSize = FVector2D(fWidth, mScreenSize.Y);
			UMGHelper::SetWidgetSize(mUImage, mActualSize);
		}
		else if(ratio1 < ratio2) //屏幕更宽
		{
			float fHeight = mScreenSize.X * mSize.Y / (float)mSize.X;
			mActualSize = FVector2D(mScreenSize.X, fHeight);
			UMGHelper::SetWidgetSize(mUImage, mActualSize);
		}

		UE_LOG(LogTemp, Log, TEXT("UMGAdapterTool mResolution: %s"), *mScreenSize.ToString());
		UE_LOG(LogTemp, Log, TEXT("UMGAdapterTool mSize: %s"), *mSize.ToString());
		UE_LOG(LogTemp, Log, TEXT("UMGAdapterTool FitBG: %.3f, %.3f"), ratio1, ratio2);
		UE_LOG(LogTemp, Log, TEXT("UMGAdapterTool mActualSize: %s"), *mActualSize.ToString());
	}
private:
	bool bInit;
};
