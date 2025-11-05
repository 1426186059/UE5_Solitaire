// Fill out your copyright notice in the Description page of Project Settings.

#include "PokerAnimationItemW.h"

void UPokerAnimationItemW::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    /*this->mIcon = Cast<UImage>(this->GetWidgetFromName(TEXT("Icon")));
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
    }*/
}

void UPokerAnimationItemW::InitPokerId(int poker_id)
{
    this->Init();
    this->SetPokerId(poker_id);
    this->Refresh();
}

void UPokerAnimationItemW::Show()
{
    this->Init();
    this->SetVisibility(ESlateVisibility::Visible);
}

void UPokerAnimationItemW::Hide()
{
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UPokerAnimationItemW::Refresh()
{
    this->Init();
    //this->mIcon->SetBrushFromAtlasInterface(AResCenter::GetSingleton()->GetPokerSprite(this->nPokerId));
}

void UPokerAnimationItemW::SetPokerId(int nPokerId1)
{
    this->nPokerId = nPokerId1;
}