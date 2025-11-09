#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "CoreMinimal.h"

class UMainUIWidget;

class TopBottomView
{
public:
    void Init(UMainUIWidget* mRootUW);
    void RefreshTopBottomUI();
    void UpdateShowHideAni(bool bAuto = true, bool bShow = true);
    void UpdateGameModeState();

    void OnBtnClicked_GameNodeBtn();
    void OnBtnClicked_UndoBtn();
    void OnBtnClicked_ForwardBtn();
private:
    UMainUIWidget* mUIRoot;

    UWidget* DownObj;
    UWidget* RightObj;

    UWidget* ChallengeHead;
    UWidget* NormalHead;
    UWidget* RankHead;

    UTextBlock* textFenShu;
    UTextBlock* textTime;
    UTextBlock* textMoves;
    UTextBlock* textRemainCount;

    UTextBlock* textIQMode;
    UWidget* goMagicwandTipCount;
    UTextBlock* textMagicwandCount;
    
    UButton* SettingBtn;
    UButton* undoNodeBtn;
    UButton* forwardNodeBtn;
    UButton* gameNode;
    UButton* themeNode;

    UButton* bt_magicwand;
    UButton* hintNodeBtn;

    bool bShowBottomUI;
};
