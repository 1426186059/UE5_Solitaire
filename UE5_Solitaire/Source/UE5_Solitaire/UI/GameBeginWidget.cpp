#include "GameBeginWidget.h"
#include "MainUIWidget.h"

void UGameBeginWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    auto clickBtn = Cast<UButton>(this->GetWidgetFromName("clickBtn"));
    clickBtn->OnClicked.AddDynamic(this, &UGameBeginWidget::OnBtnClicked);
}

void UGameBeginWidget::OnBtnClicked()
{
   /* UE_LOG(LogTemp, Log, TEXT("UGameBeginWidget OnBtnClicked"));
    if (this->bCanClickUI)
    {
        AudioHandler::GetSingleton()->PlaySound("button");
        this->Hide();
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->NewGameBegin_ForNormal(true);
    }*/
}

void UGameBeginWidget::OnShow()
{
    /*this->Init();
    this->Refresh()*/;
}

void UGameBeginWidget::Refresh()
{
   /* auto textScoreValue = Cast<UTextBlock>(GetWidgetFromName(TEXT("textScoreValue")));
    auto textTimeValue = Cast<UTextBlock>(GetWidgetFromName(TEXT("textTimeValue")));
    auto textMoveValue = Cast<UTextBlock>(GetWidgetFromName(TEXT("textMoveValue")));

    int nScore = RecordStepDataHandler::GetSingleton()->GetData()->nScore;
    int nMoveCount = RecordStepDataHandler::GetSingleton()->GetData()->nMoveCount;
    FString timeSpanStr = KKTimeTool::GetFormatTimeStr(RecordStepDataHandler::GetSingleton()->GetData()->nTime);
    
    textScoreValue->SetText(FText::AsNumber(nScore));
    textTimeValue->SetText(FText::FromString(timeSpanStr));
    textMoveValue->SetText(FText::AsNumber(nMoveCount));*/
}