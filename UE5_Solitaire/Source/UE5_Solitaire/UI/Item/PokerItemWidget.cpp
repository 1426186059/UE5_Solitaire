// Fill out your copyright notice in the Description page of Project Settings.

#include "PokerItemWidget.h"

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
    if (this->mTurnOverTween1)
    {
        this->mTurnOverTween1->cancel();
        this->mTurnOverTween1 = nullptr;
    }

    if (this->mTurnOverTween2)
    {
        this->mTurnOverTween2->cancel();
        this->mTurnOverTween2 = nullptr;
    }

    FVector2D from(0, 0);
    FVector2D to(0, -90);
    auto mTween1 = KKTween::AddTween(this, 0.15,
        [&](float fTimePercent)
        {
           // this->tranCardItemAni.localEulerAngles = KKTween::EaseFunc::easeLinear(from, to, fTimePercent)
        },
        [&]()
        {
            this->Refresh();
            //this->tranCardItemAni.localEulerAngles = Unity.Vector3(0, 90, 0);
        });

    FVector2D from2(0, 90);
    FVector2D to2(0, 0);
    auto mTween2 = KKTween::AddTween(this, 0.15,
        [&](float fTimePercent)
        {
           // this->tranCardItemAni->RenderTransform.(KKTween::EaseFunc::easeLinear(from, to, fTimePercent));
        });

    mTween1->AppendTween(mTween2);
    this->mTurnOverTween1 = mTween1;
    this->mTurnOverTween2 = mTween2;
}

void UPokerItemWidget::DoShakeAni()
{
    if (this->mShakeTween)
    {
        this->mShakeTween->cancel();
        this->mShakeTween = nullptr;
    }

    float FromRandPosX = 2 + FMath::FRand() * 3 - 1.5;
    float FromRandPosY = 2 + FMath::FRand() * 1 - 0.5;
    float ToRandPosX = -8 + FMath::FRand() * 3 - 1.5;
    float ToRandPosY = 2 + FMath::FRand() * 1 - 0.5;

    FVector2D from(FromRandPosX, FromRandPosY);
    FVector2D to(ToRandPosX, ToRandPosY);
    this->mShakeTween = KKTween::AddTween(this, 0.04, 
        [&](float fTimePercent)
        {
            UMGHelper::SetRenderPos(this->tranCardItemAni, KKTween::EaseFunc::easeLinear(from, to, fTimePercent));
        },
        [&]()
        {
            UMGHelper::SetRenderPos(this->tranCardItemAni, FVector2D::ZeroVector);
        })->SetLoopPingPong(4);
}