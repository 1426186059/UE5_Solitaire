// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/WidgetLayoutLibrary.h"
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
        UCanvasPanel* mCanvasPanel = Cast<UCanvasPanel>(mWidget->GetRootWidget());
        if (mCanvasPanel)
        {
            return GetUMGRootSzie(mCanvasPanel);
        }

        UE_LOG(LogTemp, Error, TEXT("UMGHelper GetUMGRootSzie Error!!!  Need CanvasPanel"));
        return FVector2D::ZeroVector;
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

    static void SetSizeBoxSize(UWidget* Child, const FVector2D& NewSize)
    {
        if (USizeBoxSlot* Slot = Cast<USizeBoxSlot>(Child->Slot))
        {
            
        }
    }

    static void SetSlotPos(UWidget* target, const FVector2D& pos)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        Slot->SetPosition(pos);
    }

    static FVector2D GetSlotPos(UWidget* target)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->GetPosition();
    }

    static void SetSlotAnchor(UWidget* target, const FAnchors& t)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetAnchors(t);
    }

    static void SetSlotAlignment(UWidget* target, const FVector2D& t)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetAlignment(t);
    }

    static void SetSlotSize(UWidget* target, const FVector2D& NewSize)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetSize(NewSize);
    }

    static void SetSlotAutoSize(UWidget* target, bool bAutoSize)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetAutoSize(bAutoSize);
    }

    static void SetRenderPos(UWidget* target, const FVector2D& pos)
    {
        target->SetRenderTranslation(pos);
    }

    static FVector2D GetRenderPos(UWidget* target)
    {
        return target->GetRenderTransform().Translation;
    }
    
    //这个返回的坐标，还是左上角的坐标
    static FVector2D GetRelativePos(UWidget* RootWidget, UWidget* target)
    {
        FVector2D PositionInFrom = target->GetPaintSpaceGeometry().GetLocalSize() / 2;
        FVector2D PositionInScreen = target->GetPaintSpaceGeometry().LocalToAbsolute(PositionInFrom);
        FVector2D PositionInTo = RootWidget->GetPaintSpaceGeometry().AbsoluteToLocal(PositionInScreen) - RootWidget->GetPaintSpaceGeometry().GetLocalSize() / 2;
        return PositionInTo;
    }

    static void SetRelativePos(UWidget* RootWidget, UWidget* target, FVector2D RelativePos)
    {
        FVector2D RootAbsPos = RootWidget->GetPaintSpaceGeometry().LocalToAbsolute(RelativePos);
        FVector2D targetPos = target->GetPaintSpaceGeometry().AbsoluteToLocal(RootAbsPos);
        UMGHelper::SetSlotPos(target, targetPos);
    }

    static void SetChildIndex(UWidget* target, int nIndex, UCanvasPanel* Parent = nullptr)
    {
        if (Parent == nullptr) Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->RemoveChild(target);
        Parent->InsertChildAt(nIndex, target);
    }

    static void SetAsFirstChildIndex(UWidget* target, UCanvasPanel* Parent = nullptr)
    {
        if (Parent == nullptr) Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->RemoveChild(target);
        Parent->InsertChildAt(0, target);
    }

    static void SetAsLastChildIndex(UWidget* target, UCanvasPanel* Parent = nullptr)
    {
        if (Parent == nullptr) Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->RemoveChild(target);
        Parent->AddChildToCanvas(target);
    }

private:
    UMGHelper() = delete;
    ~UMGHelper() = delete;
    UMGHelper(const UMGHelper&) = delete;
    UMGHelper& operator=(const UMGHelper&) = delete;
    UMGHelper(UMGHelper&&) = delete;
    UMGHelper& operator=(UMGHelper&&) = delete;
};
