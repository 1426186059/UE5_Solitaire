#pragma once

#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "CoreMinimal.h"

class TopBottomView
{
public:
    void Init(UCanvasPanel* t);
    void Show();
    void Hide();
    void PlayAni(TFunction<void()> finishFunc);
    void DestroyAniNode();

    /*TArray<int32> GetTableA4Color();
    TArray<FVector2D> GetTableA4WorldPos();*/
private:
    UCanvasPanel* mUIRoot;
};
