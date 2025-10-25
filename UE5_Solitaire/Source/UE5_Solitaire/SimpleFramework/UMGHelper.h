// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBoxSlot.h"


class UMGHelper
{
public:
    static FIntPoint GetImageOriginalSize(UImage* Image)
    {
        if (!Image) return FIntPoint::ZeroValue;

        const FSlateBrush& Brush = Image->GetBrush();
        UObject* Resource = Brush.GetResourceObject();

        // 情况 1：直接是 UTexture2D
        if (UTexture2D* Tex = Cast<UTexture2D>(Resource))
        {
            return FIntPoint(Tex->GetSizeX(), Tex->GetSizeY());
        }

        // 情况 2：MaterialInterface → 拿主纹理（约定参数名 MainTex）
        if (UMaterialInterface* Mat = Cast<UMaterialInterface>(Resource))
        {
            UTexture* MainTex = nullptr;
            if (Mat->GetTextureParameterValue(FName("MainTex"), MainTex))
            {
                if (UTexture2D* Tex2D = Cast<UTexture2D>(MainTex))
                {
                    return FIntPoint(Tex2D->GetSizeX(), Tex2D->GetSizeY());
                }
            }
        }

        return FIntPoint::ZeroValue;
    }

    static FVector2D GetUMGRootSzie(UUserWidget* mWidget)
    {
        auto mCanvasPanel = Cast<UCanvasPanel>(mWidget->GetRootWidget());
        return GetUMGRootSzie(mCanvasPanel);
    }

    static FVector2D GetUMGRootSzie(UCanvasPanel* mCanvasPanel)
    {
        FGeometry RootGeom = mCanvasPanel->GetCachedGeometry();   // 根节点几何
        return  RootGeom.GetLocalSize();
    }

    static void SetWidgetScale(UWidget* mUWidget, const FVector2D& NewScale)
    {
        mUWidget->SetRenderScale(NewScale);
    }

    static void SetWidgetSize(UWidget* Child, const FVector2D& NewSize)
    {
        if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Child->Slot))
        {
            Slot->SetSize(NewSize);          // 立即生效
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UMGHelper SetWidgetSize: Error"));
        }
    }

    static void SetSizeBoxSize(UWidget* Child, const FVector2D& NewSize)
    {
        if (USizeBoxSlot* Slot = Cast<USizeBoxSlot>(Child->Slot))
        {
            
        }
    }

    static void SetLocalPos(UWidget* target, FVector2D pos)
    {
        target->SetRenderTranslation(pos);
    }

private:
    // 禁止拷贝和移动（单例不应被复制）
    UMGHelper(const UMGHelper&) = delete;
    UMGHelper& operator=(const UMGHelper&) = delete;
    UMGHelper(UMGHelper&&) = delete;
    UMGHelper& operator=(UMGHelper&&) = delete;
};
