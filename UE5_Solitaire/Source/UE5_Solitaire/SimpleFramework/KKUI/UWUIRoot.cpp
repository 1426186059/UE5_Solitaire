// Fill out your copyright notice in the Description page of Project Settings.


#include "UWUIRoot.h"

void UWUIRoot::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    UE_LOG(LogTemp, Log, TEXT("UWUIRoot Init"));

    this->Layer_Background = Cast<UCanvasPanel>(this->GetWidgetFromName("Layer_Background"));
    this->Layer_Base = Cast<UCanvasPanel>(this->GetWidgetFromName("Layer_Base"));
    this->Layer_Loading = Cast<UCanvasPanel>(this->GetWidgetFromName("Layer_Loading"));
    this->Layer_Popup = Cast<UCanvasPanel>(this->GetWidgetFromName("Layer_Popup"));
    this->Layer_Tip = Cast<UCanvasPanel>(this->GetWidgetFromName("Layer_Tip"));
    this->Layer_Debug = Cast<UCanvasPanel>(this->GetWidgetFromName("Layer_Debug"));

    this->AddToViewport(1);
    Cast<UCanvasPanelSlot>(this->Layer_Background->Slot)->SetZOrder(0);
    Cast<UCanvasPanelSlot>(this->Layer_Base->Slot)->SetZOrder(1);
    Cast<UCanvasPanelSlot>(this->Layer_Loading->Slot)->SetZOrder(2);
    Cast<UCanvasPanelSlot>(this->Layer_Popup->Slot)->SetZOrder(3);
    Cast<UCanvasPanelSlot>(this->Layer_Tip->Slot)->SetZOrder(4);
    Cast<UCanvasPanelSlot>(this->Layer_Debug->Slot)->SetZOrder(5);
}
