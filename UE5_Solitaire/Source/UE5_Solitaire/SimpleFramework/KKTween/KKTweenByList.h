#pragma once

#include "CoreMinimal.h"
#include "KKTween.h"

namespace KKTween
{
    class AKKTweenMgr;

    class KKTweenByList
    {
    private:
        ObjectPool mItemPool;
        TArray<KKTweenItem*> mTweenT;
        AKKTweenMgr* defaultBindObj;
    public:
        KKTweenByList(AKKTweenMgr* mDefaultBindObj);

        void Update(float DeltaTime);
        KKTweenItem* AddTween(
            float time,
            Action_Float_Delegate updateFunc = nullptr,
            ActionDelegate finishFunc = nullptr);

        KKTweenItem* AddTween(
            UObject* obj,
            float time,
            Action_Float_Delegate updateFunc = nullptr,
            ActionDelegate finishFunc = nullptr);

    };
}
