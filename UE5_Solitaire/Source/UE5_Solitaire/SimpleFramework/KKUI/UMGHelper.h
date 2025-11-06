// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PaperSprite.h"

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

    static FVector2D GetScreenLocalSize(UUserWidget*  mWidget)
    {
        FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(mWidget);
        if(mWidget == nullptr)
        {
            return ScreenSize;
        }

        const FGeometry& Geo = mWidget->GetPaintSpaceGeometry();

        // 获取本地 (0,0) 和 (1,1) 对应的屏幕位置
        FVector2D ScreenOrigin = Geo.LocalToAbsolute(FVector2D::Zero());
        FVector2D ScreenUnit = Geo.LocalToAbsolute(FVector2D(1.0f, 1.0f));

        // 计算每单位本地尺寸对应的屏幕像素（缩放因子）
        FVector2D Scale = ScreenUnit - ScreenOrigin; // 注意：可能为负（镜像）

        // 避免除零
        float InvScaleX = FMath::Abs(Scale.X) > KINDA_SMALL_NUMBER ? 1.0f / Scale.X : 1.0f;
        float InvScaleY = FMath::Abs(Scale.Y) > KINDA_SMALL_NUMBER ? 1.0f / Scale.Y : 1.0f;

        return FVector2D(ScreenSize.X * InvScaleX, ScreenSize.Y * InvScaleY);
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

    static void SetWidgetScale(UWidget* mUWidget, FVector2D NewScale)
    {
        mUWidget->SetRenderScale(NewScale);
    }

    static void SetSizeBoxSize(UWidget* Child, FVector2D NewSize)
    {
        if (USizeBoxSlot* Slot = Cast<USizeBoxSlot>(Child->Slot))
        {
            
        }
    }

    static void SetSlotPos(UWidget* target, FVector2D pos)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        Slot->SetPosition(pos);
    }

    static FVector2D GetSlotPos(UWidget* target)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->GetPosition();
    }

    static void SetSlotAnchor(UWidget* target, FAnchors t)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        if (Slot == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UMGHelper SetSlotPos Error!!!  Need UCanvasPanelSlot"));
            return;
        }
        return Slot->SetAnchors(t);
    }

    static void SetSlotAlignment(UWidget* target, FVector2D t)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetAlignment(t);
    }

    static void SetSlotSize(UWidget* target, FVector2D NewSize)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetSize(NewSize);
    }

    static void SetSlotAutoSize(UWidget* target, bool bAutoSize)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetAutoSize(bAutoSize);
    }

    static void SetRenderPos(UWidget* target, FVector2D pos)
    {
        target->SetRenderTranslation(pos);
    }

    static FVector2D GetRenderPos(UWidget* target)
    {
        return target->GetRenderTransform().Translation;
    }
    
    //这个返回的坐标，还是左上角的坐标
    static FVector2D GetRLocalPos(UWidget* Me, UWidget* otherTarget)
    {
        FVector2D AbsolutePos = GetRAbsolutePos(otherTarget);
        return GetRLocalPos(Me, AbsolutePos);
    }

    static FVector2D GetRLocalPos(UWidget* Me, FVector2D AbsolutePos)
    {
        auto mUCanvasPanelSlot = Cast<UCanvasPanelSlot>(Me->Slot);
        FVector2D OffsetPos;
        if (mUCanvasPanelSlot)
        {
            OffsetPos = mUCanvasPanelSlot->GetAlignment() * Me->GetPaintSpaceGeometry().GetLocalSize();
        }

        return Me->GetPaintSpaceGeometry().AbsoluteToLocal(AbsolutePos) - OffsetPos;
    }

    static FVector2D GetRAbsolutePos(UWidget* Parent, FVector2D localPos)
    {
        auto mUCanvasPanelSlot = Cast<UCanvasPanelSlot>(Parent->Slot);
        FVector2D OffsetPos;
        if (mUCanvasPanelSlot)
        {
            OffsetPos = mUCanvasPanelSlot->GetAlignment() * Parent->GetPaintSpaceGeometry().GetLocalSize();
        }
        return Parent->GetPaintSpaceGeometry().LocalToAbsolute(localPos + OffsetPos);
    }

    static FVector2D GetRAbsolutePos(UWidget* target)
    {
        auto mUCanvasPanelSlot = Cast<UCanvasPanelSlot>(target->Slot);
        FVector2D OffsetPos;
        if (mUCanvasPanelSlot)
        {
            OffsetPos = mUCanvasPanelSlot->GetAlignment() * target->GetPaintSpaceGeometry().GetLocalSize();
        }
        return target->GetPaintSpaceGeometry().LocalToAbsolute(OffsetPos);
    }

    static void SetChildIndex(UWidget* target, int nIndex)
    {
        auto Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->ShiftChild(nIndex, target);
    }

    static void SetAsFirstChildIndex(UWidget* target)
    {
        auto Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->ShiftChild(0, target);
    }

    static void SetAsLastChildIndex(UWidget* target)
    {
        auto Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->ShiftChild(Parent->GetChildrenCount(), target);
    }

    static void SetParent(UWidget* target, UPanelWidget* Parent)
    {
        if (target->GetParent() != Parent)
        {
            Parent->AddChild(target);
        }
    }

    static void SetZOrder(UWidget* target, int nOrder)
    {
        UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(target->Slot);
        return Slot->SetZOrder(nOrder);
    }

    static void AutoSetChildZOrder(UCanvasPanel* mParent)
    {
        for (int i = 0; i < mParent->GetChildrenCount(); i++)
        {
            SetZOrder(mParent->GetChildAt(i), i);
        }
    }

    static void SetChildZOrder(UWidget* target, int nIndex)
    {
        auto Parent = Cast<UCanvasPanel>(target->GetParent());
        int nChildCount = Parent->GetChildrenCount();
        Parent->ShiftChild(nIndex, target);
        AutoSetChildZOrder(Parent);
    }

    static void SetChildFirstZOrder(UWidget* target)
    {
        auto Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->ShiftChild(0, target);

        if (true)
        {
            AutoSetChildZOrder(Parent);
        }
        else
        {
            int nFirstZOrder = 0;
            if (Parent->GetChildrenCount() >= 2)
            {
                UWidget* mFirstWidget = Parent->GetChildAt(1);
                nFirstZOrder = Cast<UCanvasPanelSlot>(mFirstWidget->Slot)->GetZOrder();
            }

            if (nFirstZOrder <= -UINT16_MAX)
            {
                AutoSetChildZOrder(Parent);
            }
            else
            {
                SetZOrder(target, --nFirstZOrder);
            }
        }
    }

    static void SetChildLastZOrder(UWidget* target)
    {
        auto Parent = Cast<UCanvasPanel>(target->GetParent());
        Parent->ShiftChild(Parent->GetChildrenCount(), target);

        if (false)
        {
            AutoSetChildZOrder(Parent);
        }
        else
        {
            int nLastZOrder = 0;
            if (Parent->GetChildrenCount() >= 2)
            {
                UWidget* mLastWidget = Parent->GetChildAt(Parent->GetChildrenCount() - 2);
                nLastZOrder = Cast<UCanvasPanelSlot>(mLastWidget->Slot)->GetZOrder();
            }

            if (nLastZOrder >= UINT16_MAX)
            {
                AutoSetChildZOrder(Parent);
            }
            else
            {
                SetZOrder(target, ++nLastZOrder);
            }
        }
    }

    static void RemoveAllChildren(UCanvasPanel* mParent)
    {
        while (mParent->GetChildrenCount() > 0)
        {
            mParent->RemoveChildAt(0);
        }
    }

    static void DestroyWidget(UWidget* t)
    {
        if (t->IsInViewport() || t->GetParent() != nullptr)
        {
            t->RemoveFromParent(); //销毁
        }
    }

private:
    UMGHelper() = delete;
    ~UMGHelper() = delete;
    UMGHelper(const UMGHelper&) = delete;
    UMGHelper& operator=(const UMGHelper&) = delete;
    UMGHelper(UMGHelper&&) = delete;
    UMGHelper& operator=(UMGHelper&&) = delete;
};
