// Fill out your copyright notice in the Description page of Project Settings.

#include "PokerItemWidget.h"
#include "../MainUIWidget.h"

FReply UPokerItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnMouseButtonDown "));
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        //return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
        return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPokerItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragDetected "));
    
    UPokerItemDragDropOperation* DragOp = NewObject<UPokerItemDragDropOperation>();
    DragOp->WidgetSource = this;
    DragOp->DragOffset = InGeometry.GetAbsolutePosition() - InMouseEvent.GetScreenSpacePosition();

    //DragOp->DefaultDragVisual = CreateDragVisual();
    //DragOp->Pivot = EDragPivot::MouseDown;

    OutOperation = DragOp;

    this->bInDrag = true;
    this->beginPos = UMGHelper::GetSlotPos(this);
    //AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->CardHintEffectPool->Reset();
    AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->TellRobot_PlayerAlive();
    AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->OnDragBegin(this);
}

void UPokerItemWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragEnter "));
}

void UPokerItemWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragLeave "));
}

bool UPokerItemWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragOver "));

    UPokerItemDragDropOperation* DragOp = Cast<UPokerItemDragDropOperation>(InOperation);

    FVector2D localPos1 = this->GetParent()->GetCachedGeometry().AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
    FVector2D localPos2 = this->GetParent()->GetCachedGeometry().AbsoluteToLocal(InDragDropEvent.GetLastScreenSpacePosition());
    FVector2D OffsetPos = localPos1 - localPos2;

    UMGHelper::SetSlotPos(this, UMGHelper::GetSlotPos(this) + OffsetPos);
    AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->OnDrag(this);
    AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->TellRobot_PlayerAlive();

    return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

bool UPokerItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDrop "));

    AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->OnDragEndToMovePokerPos(this);
    AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->TellRobot_PlayerAlive();
    KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UPokerItemWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;
    this->mIcon = Cast<UImage>(this->GetWidgetFromName(TEXT("Icon")));
    if (!mIcon)
    {
        UE_LOG(LogTemp, Error, TEXT("mIcon == null"));
        return;
    }

    this->tranCardItemAni = this->GetWidgetFromName("CardItemAni");
    if (!tranCardItemAni)
    {
        UE_LOG(LogTemp, Error, TEXT("tranCardItemAni == null"));
        return;
    }
}

void UPokerItemWidget::Show()
{
    this->Init();
    this->SetVisibility(ESlateVisibility::Visible);
}

void UPokerItemWidget::Hide()
{
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UPokerItemWidget::Refresh()
{
    this->Init();
    if (this->orTurnOverStateIsTrue())
    {
        this->mIcon->SetBrushFromAtlasInterface(AResCenter::GetSingleton()->GetPokerSprite(this->nPokerId));
    }
    else
    {
        this->mIcon->SetBrushFromAtlasInterface(AResCenter::GetSingleton()->GetPokerSprite(-1));
    }
}

void UPokerItemWidget::SetPokerId(int nPokerId1)
{
    this->nPokerId = nPokerId1;
}

void UPokerItemWidget::SetTurnOverState(bool bShow, int nStepIndex)
{
    if (this->bTurnOverState == bShow)
    {
        return;
    }

    this->bTurnOverState = bShow;
    if (bShow)
    {
        if (nStepIndex == -1)
        {
            //nStepIndex = AKKUIMgr::GetSingleton()->GetNowRecordStepIndex();
        }

        this->nStepIndex_ForFirstShowPokerId = nStepIndex;
    }
    else
    {
        this->nStepIndex_ForFirstShowPokerId = -1;
    }
}

bool UPokerItemWidget::orTurnOverStateIsTrue()
{
    return this->bTurnOverState;
}

void UPokerItemWidget::ForceShowBackUI()
{
    this->mIcon->SetBrushFromAtlasInterface(AResCenter::GetSingleton()->GetPokerSprite(-1));
}

void UPokerItemWidget::SetEventTriggerState(bool bCanClick)
{
    if (this->IsFocusable() != bCanClick)
    {
        this->SetIsFocusable(bCanClick);
    }
}

//--²¥·Å·­ÅÆ¶¯»­
void UPokerItemWidget::PlayTurnOverAni()
{
    this->mTurnOverTween1.Cancel();
    this->mTurnOverTween2.Cancel();

    FVector2D from(1, 1);
    FVector2D to(0, 1);
    auto mTween1 = KKTween::GetHandle(KKTween::AddTween(this, 0.15,
        [=,this](float fTimePercent)
        {
            this->tranCardItemAni->SetRenderScale(KKTween::EaseFunc::easeLinear(from, to, fTimePercent));
        },
        [=,this]()
        {
            this->Refresh();
            this->tranCardItemAni->SetRenderScale(to);
        }));

    FVector2D from2(0, 1);
    FVector2D to2(1, 1);
    auto mTween2 = KKTween::GetHandle(KKTween::AddTween(this, 0.15,
        [=,this](float fTimePercent)
        {
            this->tranCardItemAni->SetRenderScale(KKTween::EaseFunc::easeLinear(from2, to2, fTimePercent));
        }));

    mTween1.AppendTween(mTween2);
    this->mTurnOverTween1 = mTween1;
    this->mTurnOverTween2 = mTween2;
}

void UPokerItemWidget::DoShakeAni()
{
    if (this->mShakeTween.IsValid())
    {
        this->mShakeTween.Cancel();
    }

    float FromRandPosX = 2 + FMath::FRand() * 3 - 1.5;
    float FromRandPosY = 2 + FMath::FRand() * 1 - 0.5;
    float ToRandPosX = -8 + FMath::FRand() * 3 - 1.5;
    float ToRandPosY = 2 + FMath::FRand() * 1 - 0.5;

    FVector2D from(FromRandPosX, FromRandPosY);
    FVector2D to(ToRandPosX, ToRandPosY);
    this->mShakeTween = KKTween::GetHandle(KKTween::AddTween(this, 0.04,
        [=,this](float fTimePercent)
        {
            UMGHelper::SetRenderPos(this->tranCardItemAni, KKTween::EaseFunc::easeLinear(from, to, fTimePercent));
        },
        [=,this]()
        {
            UMGHelper::SetRenderPos(this->tranCardItemAni, FVector2D::ZeroVector);
        })->SetLoopPingPong(4));
}