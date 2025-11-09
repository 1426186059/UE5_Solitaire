// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLoseWidget.h"

void UGameLoseWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

}

void UGameLoseWidget::CheckFirstLayoutOkToShow()
{
    if (orFirstLayoutFinish())
    {
        if (this->bShowUI)
        {
            this->SetVisibility(ESlateVisibility::Visible);
        }
    }
}