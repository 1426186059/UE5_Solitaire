#include "UE5_Solitaire/UI/Item/PokerAnimationItemW.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "UE5_Solitaire/GameLogic/GameConst.h"
#include "CoreMinimal.h"
#include "GameWinAniMgr.generated.h"

UCLASS()
class UGameWinAniMgr :public UObject
{

public:
    public AnimationType nAniType = AnimationType.Default;
    GameObject animationNode = null;

public:
    void UGameWinAniMgr::showAnimation(SolitaireWinAnimationType at, TArray<int32> colors, FVector2D startPt_w, int offsetX, TFunction<void()> callback = nullptr);     
    void RemoveLastWinAniNode();
}



