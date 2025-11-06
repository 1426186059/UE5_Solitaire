#include "GMWidget.h"
#include "MainUIWidget.h"

void UGMWidget::OnCreate()
{
    Super::OnCreate();
    UMGHelper::SetParent(this, AKKUIMgr::GetSingleton()->GetRootWidget()->Layer_Debug);
    UMGHelper::SetSlotAnchor(this, FAnchors(0, 0, 1, 1));
    UMGHelper::SetSlotAlignment(this, FVector2D(0.5));
    UMGHelper::SetSlotPos(this, FVector2D(0));
    UMGHelper::SetSlotSize(this, FVector2D(0));
}

void UGMWidget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    //auto StageRewardBtn = Cast<UButton>(this->GetWidgetFromName(TEXT("StageRewardBtn")));
    //StageRewardBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_StageRewardBtn);

    auto WinAnimationBtn = Cast<UButton>(this->GetWidgetFromName(TEXT("WinAnimationBtn")));
    WinAnimationBtn->OnClicked.AddDynamic(this, &UGMWidget::OnBtnClicked_WinAnimationBtn);

    auto AutoShouPaiBtn = Cast<UButton>(this->GetWidgetFromName(TEXT("AutoShouPaiBtn")));
    AutoShouPaiBtn->OnClicked.AddDynamic(this, &UGMWidget::OnBtnClicked_AutoShouPaiBtn);

    /*auto GuideFastPlayBtn = Cast<UButton>(this->GetWidgetFromName(TEXT("GuideFastPlayBtn")));
    GuideFastPlayBtn->OnClicked.AddDynamic(this, &UMainUIWidget::OnBtnClicked_StageRewardBtn);*/
}

void UGMWidget::Show()
{
    Super::Show();
    this->Init();
}

void UGMWidget::Hide(bool bDestory)
{
    Super::Hide(bDestory);
}

void UGMWidget::Refresh()
{
    Super::Refresh();
}

void UGMWidget::CheckFirstLayoutOkToShow()
{
    if (this->bShowUI)
    {
        this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

void UGMWidget::OnBtnClicked_AutoShouPaiBtn()
{
    UE_LOG(LogTemp, Log, TEXT("UGMWidget OnBtnClicked_AutoShouPaiBtn"));
    auto mMainUIWidget = AKKUIMgr::GetSingleton()->Get<UMainUIWidget>();
    ensureMsgf(mMainUIWidget, TEXT("mMainUIWidget == null"));

    mMainUIWidget->DoA4AllDataOp();
    mMainUIWidget->AutoShouPai([]()
        {

        });
}

void UGMWidget::OnBtnClicked_WinAnimationBtn()
{
    UE_LOG(LogTemp, Log, TEXT("UGMWidget OnBtnClicked_WinAnimationBtn"));
    auto mMainUIWidget = AKKUIMgr::GetSingleton()->Get<UMainUIWidget>();

    ensureMsgf(mMainUIWidget, TEXT("mMainUIWidget == null"));
    ensureMsgf(mMainUIWidget->GameWinAniMgr, TEXT("mMainUIWidget->GameWinAniMgr == null"));

    if (mMainUIWidget && mMainUIWidget->GameWinAniMgr)
    {
        mMainUIWidget->GameWinAniMgr->PlayAni([]()
            {
                AKKUIMgr::GetSingleton()->Get<UMainUIWidget>()->GameWinAniMgr->DestroyAniNode();
            });
    }
}
