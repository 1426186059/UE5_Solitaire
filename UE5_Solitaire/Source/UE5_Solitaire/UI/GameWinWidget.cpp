#include "GameWinWidget.h"

void UGameWinWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    //mUIRoot = Cast<UUserWidget>(GetWidgetFromName(TEXT("InitSceneUIBP")));
    //if (!mUIRoot)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("mUIRoot == null"));
    //}

    //mUProgressBar = Cast<UProgressBar>(mUIRoot->GetWidgetFromName(TEXT("ProgressBar_139")));
    //if (!mUProgressBar)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to find ProgressBar by name 'ProgressBar'!"));
    //}
}

void UGameWinWidget::CheckFirstLayoutOkToShow()
{
    if (orFirstLayoutFinish())
    {
        if (this->bShowUI)
        {
            this->SetVisibility(ESlateVisibility::Visible);
        }
    }
}