// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLoseWidget.h"
#include "MainUIWidget.h"

void UGameLoseWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    auto cancelBtn = Cast<UButton>(this->GetWidgetFromName("cancelBtn"));
    cancelBtn->OnClicked.AddDynamic(this, &UGameLoseWidget::OnBtnClicked_Cancel);

    auto newGameBtn = Cast<UButton>(this->GetWidgetFromName("newGameBtn"));
    newGameBtn->OnClicked.AddDynamic(this, &UGameLoseWidget::OnBtnClicked_NewGame);
}

void UGameLoseWidget::OnBtnClicked_Cancel()
{
    UE_LOG(LogTemp, Log, TEXT("UGameWinWidget OnBtnClicked_Cancel"));
    if (this->bCanClickUI)
    {
        this->bCanClickUI = false;
        AudioHandler::GetSingleton()->PlaySound("button");
        this->Hide();
    }
}

void UGameLoseWidget::OnBtnClicked_NewGame()
{
    UE_LOG(LogTemp, Log, TEXT("UGameWinWidget OnBtnClicked_NewGame"));
    if (this->bCanClickUI)
    {
        this->bCanClickUI = false;
        AudioHandler::GetSingleton()->PlaySound("button");
        this->Hide();
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->NewGameBegin(true);
    }
}

void UGameLoseWidget::OnShow()
{
    this->Init();
    this->Refresh();
    this->bCanClickUI = true;
}

void UGameLoseWidget::Refresh()
{
    
}