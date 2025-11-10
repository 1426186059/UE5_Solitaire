#include "GameBeginWidget.h"
#include "MainUIWidget.h"

void UGameBeginWidget::OnCreate()
{
    UMGHelper::SetParent(this, AKKUIMgr::GetSingleton()->GetRootWidget()->Layer_Popup);
    UMGHelper::SetSlotAnchor(this, FAnchors(0, 0, 1, 1));
    UMGHelper::SetSlotAlignment(this, FVector2D(0.5));
    UMGHelper::SetSlotPos(this, FVector2D(0));
    UMGHelper::SetSlotSize(this, FVector2D(0));
}

void UGameBeginWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    auto blockEvent = Cast<UButton>(this->GetWidgetFromName("blockEvent"));
    blockEvent->OnClicked.AddDynamic(this, &UGameBeginWidget::OnBtnClicked_Hide);

    auto rePlayNode = Cast<UButton>(this->GetWidgetFromName("rePlayNode"));
    rePlayNode->OnClicked.AddDynamic(this, &UGameBeginWidget::OnBtnClicked_ReStart);

    auto newGameNode = Cast<UButton>(this->GetWidgetFromName("newGameNode"));
    newGameNode->OnClicked.AddDynamic(this, &UGameBeginWidget::OnBtnClicked_NewGame);

    auto Draw1Btn = Cast<UButton>(this->GetWidgetFromName("Draw1Btn"));
    Draw1Btn->OnClicked.AddDynamic(this, &UGameBeginWidget::OnBtnClicked_Draw1);
    auto Draw2Btn = Cast<UButton>(this->GetWidgetFromName("Draw2Btn"));
    Draw2Btn->OnClicked.AddDynamic(this, &UGameBeginWidget::OnBtnClicked_Draw2);
    auto Draw3Btn = Cast<UButton>(this->GetWidgetFromName("Draw3Btn"));
    Draw3Btn->OnClicked.AddDynamic(this, &UGameBeginWidget::OnBtnClicked_Draw3);

}

void UGameBeginWidget::OnBtnClicked_Hide()
{
    if (this->bCanClickUI)
    {
        this->bCanClickUI = false;
        AudioHandler::GetSingleton()->PlaySound("button");
        this->HideWithAni();
    }
}

void UGameBeginWidget::OnBtnClicked_ReStart()
{
    if (this->bCanClickUI)
    {
        this->bCanClickUI = false;
        AudioHandler::GetSingleton()->PlaySound("button");
        this->HideWithAni();
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->NewGameBegin_ForRePlay();
    }
}

void UGameBeginWidget::OnBtnClicked_NewGame()
{
    if (this->bCanClickUI)
    {
        this->bCanClickUI = false;
        AudioHandler::GetSingleton()->PlaySound("button");
        this->HideWithAni();
        AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->NewGameBegin_ForNormal(true);
    }
}

void UGameBeginWidget::OnBtnClicked_Draw1()
{
    UE_LOG(LogTemp, Log, TEXT("TopBottomView OnBtnClicked_Draw1"));
    AudioHandler::GetSingleton()->PlaySound("button");
    DataCenter::GetSingleton()->SetDrawCount(1);
    this->UpdateSwitchDraw3State();
}

void UGameBeginWidget::OnBtnClicked_Draw2()
{
    UE_LOG(LogTemp, Log, TEXT("TopBottomView OnBtnClicked_Draw2"));
    AudioHandler::GetSingleton()->PlaySound("button");
    DataCenter::GetSingleton()->SetDrawCount(2);
    this->UpdateSwitchDraw3State();
}

void UGameBeginWidget::OnBtnClicked_Draw3()
{
    UE_LOG(LogTemp, Log, TEXT("TopBottomView OnBtnClicked_Draw3"));
    AudioHandler::GetSingleton()->PlaySound("button");
    DataCenter::GetSingleton()->SetDrawCount(3);
    this->UpdateSwitchDraw3State();
}

void UGameBeginWidget::OnShow()
{
    this->Init();
    this->Refresh();

    this->bCanClickUI = false;
    UMGAni::PlayShowDownToUpAni(this, true, [this]()
        {
            this->bCanClickUI = true;
        });
}

void UGameBeginWidget::HideWithAni(bool bDestroy)
{
    this->bCanClickUI = false;
    UMGAni::PlayShowDownToUpAni(this, false, [=, this]()
        {
            this->Hide(bDestroy);
        });
}

void UGameBeginWidget::Refresh()
{
    this->UpdateSwitchDraw3State();
}

void UGameBeginWidget::UpdateSwitchDraw3State()
{
    int nSelectIndex = DataCenter::GetSingleton()->GetData()->nDrawCount - 1;
    ensure(nSelectIndex >= 0 && nSelectIndex < 3);

    auto Draw1 = Cast<UCanvasPanel>(this->GetWidgetFromName("Draw1"));
    auto Draw2 = Cast<UCanvasPanel>(this->GetWidgetFromName("Draw2"));
    auto Draw3 = Cast<UCanvasPanel>(this->GetWidgetFromName("Draw3"));

    TArray<UCanvasPanel*> mList = { Draw1, Draw2, Draw3 };
    for (int i = 0; i < mList.Num(); i++)
    {
        if (i == nSelectIndex)
        {
            mList[i]->GetChildAt(0)->SetVisibility(ESlateVisibility::Hidden);
            mList[i]->GetChildAt(1)->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            mList[i]->GetChildAt(0)->SetVisibility(ESlateVisibility::Visible);
            mList[i]->GetChildAt(1)->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}