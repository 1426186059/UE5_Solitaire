// Fill out your copyright notice in the Description page of Project Settings.

#include "PokerItemWidget.h"
#include "../MainUIWidget.h"

void UPokerItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    this->OnDrag();
}

FReply UPokerItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnMouseButtonDown %d: "), this->nPokerId);
    //if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    //{
    //    this->OnDragBegin();
    //    return FReply::Handled().CaptureMouse(TakeWidget());
    //}
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UPokerItemWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnMouseButtonUp %d: "), this->nPokerId);
    //if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    //{
    //    this->OnDragEnd();
    //    return FReply::Handled().ReleaseMouseCapture();
    //}
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UPokerItemWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UPokerItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragDetected %d: "), this->nPokerId);

    //UPokerItemDragDropOperation* DragOp = NewObject<UPokerItemDragDropOperation>();
    //DragOp->WidgetSource = this;
    //DragOp->DragOffset = InGeometry.GetAbsolutePosition() - InMouseEvent.GetScreenSpacePosition();
    //DragOp->BeginScreenSpacePos = InMouseEvent.GetScreenSpacePosition();
    //DragOp->DragStartSlotPos = UMGHelper::GetSlotPos(this);
    ////DragOp->DefaultDragVisual = this;
    ////DragOp->Pivot = EDragPivot::CenterCenter;
    //OutOperation = DragOp;

    //this->bInDrag = true;
    ////AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->CardHintEffectPool->Reset();
    //AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->TellRobot_PlayerAlive();
    //AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->OnDragBegin(this);
}

void UPokerItemWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragEnter %d:"), this->nPokerId);
}

void UPokerItemWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragLeave %d:"), this->nPokerId);
}

bool UPokerItemWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
   // this->CurrentTouchPosition = InDragDropEvent.GetScreenSpacePosition();
   // UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragOver %d:"), this->nPokerId);
    return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

void UPokerItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDragCancelled %d: "), this->nPokerId);
}

bool UPokerItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnDrop  %d: "), this->nPokerId);
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


FReply UPokerItemWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    uint32 FingerIndex = InGestureEvent.GetPointerIndex();
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnTouchStarted :%d, FingerIndex: %d"), this->nPokerId, FingerIndex);
    if (FingerIndex == 0)
    {
        this->CurrentTouchPosition = InGestureEvent.GetScreenSpacePosition();
        this->OnDragBegin();
        return FReply::Handled().CaptureMouse(TakeWidget());
    }
    return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UPokerItemWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    this->CurrentTouchPosition = InGestureEvent.GetScreenSpacePosition();
    return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UPokerItemWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget NativeOnTouchEnded %d: "), this->nPokerId);
    uint32 FingerIndex = InGestureEvent.GetPointerIndex();
    if (FingerIndex == 0)
    {
        this->CurrentTouchPosition = InGestureEvent.GetScreenSpacePosition();
        this->OnDragEnd();
        return FReply::Handled().ReleaseMouseCapture();
    }
    return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}

void UPokerItemWidget::OnDragBegin()
{
    if (!this->bInDrag)
    {
        this->bInDrag = true;
        this->beginPos = UMGHelper::GetSlotPos(this);
        this->beginScreenSpacePos = this->CurrentTouchPosition;
        //AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->CardHintEffectPool->Reset();
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->TellRobot_PlayerAlive();
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->OnDragBegin(this);
    }
}

void UPokerItemWidget::OnDrag()
{
    //FVector2D  MousePos = UWidgetLayoutLibrary::GetMousePositionOnPlatform();
    if (this->bInDrag)
    {
        //UE_LOG(LogTemp, Log, TEXT("UPokerItemWidget OnDrag"));

        FVector2D  MousePos = this->CurrentTouchPosition;
        FVector2D localPos1 = this->GetParent()->GetCachedGeometry().AbsoluteToLocal(MousePos);
        FVector2D localPos2 = this->GetParent()->GetCachedGeometry().AbsoluteToLocal(this->beginScreenSpacePos);
        FVector2D OffsetPos = localPos1 - localPos2;
        UMGHelper::SetSlotPos(this, this->beginPos + OffsetPos);
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->OnDrag(this);
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->TellRobot_PlayerAlive();
    }
}

void UPokerItemWidget::OnDragEnd()
{
    if (this->bInDrag)
    {
        this->bInDrag = false;
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->OnDragEndToMovePokerPos(this);
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->TellRobot_PlayerAlive();
        KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_RefreshTopBottomUI)->Broadcast(nullptr);
    }
}

void UPokerItemWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    this->mIcon = Cast<UImage>(this->GetWidgetFromName(TEXT("Icon")));
    if (!this->mIcon)
    {
        UE_LOG(LogTemp, Error, TEXT("mIcon == null"));
        return;
    }

    this->tranCardItemAni = this->GetWidgetFromName("CardItemAni");
    if (!this->tranCardItemAni)
    {
        UE_LOG(LogTemp, Error, TEXT("tranCardItemAni == null"));
        return;
    }
}

void UPokerItemWidget::Show()
{
    this->Init();
    this->bShow = true;
    this->UpdateVisibaleState();
}

void UPokerItemWidget::Hide()
{
    this->bShow = false;
    this->UpdateVisibaleState();
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

void UPokerItemWidget::SetTurnOverState(bool orShow, int nStepIndex)
{
    if (this->bTurnOverState == orShow)
    {
        return;
    }

    this->bTurnOverState = orShow;
    if (orShow)
    {
        if (nStepIndex == -1)
        {
            nStepIndex = AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GetNowRecordStepIndex();
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

void UPokerItemWidget::SetEventTriggerState(bool canClick)
{
    this->bCanClick = canClick;
    this->UpdateVisibaleState();
}

void UPokerItemWidget::UpdateVisibaleState()
{
    if (this->bShow)
    {
        if (this->bCanClick)
        {
            this->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            this->SetVisibility(ESlateVisibility::HitTestInvisible); //不可点击和拖拽。
        }
    }
    else
    {
        this->SetVisibility(ESlateVisibility::Hidden);
    }
}

//--播放翻牌动画
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