#pragma once

#include "UE5_Solitaire/SimpleFramework/KKEventMgr.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/UMGHelper.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

class GameWinAniMgr
{
private:
    UWidget* go;
public:
    void Init(UWidget* go);
    void Show();
    void Hide();
    void PlayAni(TFunction<void()> finishFunc);
    void DestroyAniNode();
};
