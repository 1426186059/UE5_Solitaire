#pragma once

#include "WinAnimation/WinAniBaseUW.h"
#include "Item/PokerAnimationItemW.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "CoreMinimal.h"
#include "GameWinAniMgr.generated.h"

UCLASS()
class UGameWinAniMgr :public UObject
{
    GENERATED_BODY()

public:
    UCanvasPanel* go;
    KKWidgetPool<UPokerAnimationItemW>* mCardItemPool;
    TFunction<void()> mFinishFunc;
    
    UWinAniBaseUW* mWinAniInstance;

public:
    void Init(UCanvasPanel* t);
    void Show();
    void Hide();
    void PlayAni(TFunction<void()> finishFunc);
    void DestroyAniNode();

    TArray<int32> GetTableA4Color();
    TArray<FVector2D> GetTableA4WorldPos();
};
