#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "CoreMinimal.h"

class UMainUIWidget;

class TopBottomView
{
public:
    void Init(UMainUIWidget* mRootUW);
    void RefreshTopBottomUI();
    void UpdateShowHideAni(bool bShow);

private:
    UMainUIWidget* mRootUW;
    UCanvasPanel* mUIRoot;

    UTextBlock* textFenShu;
    UTextBlock* textTime;
    UTextBlock* textMoves;
    UTextBlock* textRemainCount;

    UTextBlock* textIQMode;
    UWidget* goMagicwandTipCount;
    UTextBlock* textMagicwandCount;
    UTextBlock* textRemainCount;
    
    UButton* SettingBtn;
    UButton* undoNodeBtn;
    UButton* forwardNodeBtn;
    UButton* gameNode;
    UButton* themeNode;

    UButton* bt_magicwand;
    UButton* hintNodeBtn;
};
