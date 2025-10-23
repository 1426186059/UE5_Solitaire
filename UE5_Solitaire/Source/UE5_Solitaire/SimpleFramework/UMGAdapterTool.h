// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Widget.h"
#include "Components/Image.h"

#include "KKSingleton.h"
#include "UMGHelper.h"

class UMGAdapterTool : public KKSingleton<UMGAdapterTool>
{
public:
	void Init()
	{
		if (bInit) return;
		bInit = true;
	}

	//void FitBG(UImage* mUImage) //这个的话，根据原有精灵尺寸，等比例缩放
	//{
	//	FIntPoint mResolution = GEngine->GetGameUserSettings()->GetScreenResolution();

	//	float ratio1 = mResolution.Y / mResolution.X;
	//	//float ratio2 = mWidget.Y / mResolution.X;

	//	FIntPoint mSize = UMGHelper::GetImageOriginalSize(mUImage);
	//}

	void FitBG(UWidget* mWidget)
	{
		FIntPoint mResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
		FVector2D mBGXY = mWidget->GetDesiredSize();   // FVector2D
		
		float ratio1 = mResolution.Y / mResolution.X;
		float ratio2 = mBGXY.Y / mBGXY.X;

		if (ratio1 > ratio2) //屏幕更长了
		{
			float scale = ratio1 / ratio2;
			UMGHelper::SetWidgetScale(mWidget, FVector2D(scale, scale));
		}
	}
	
private:
	bool bInit;
};
