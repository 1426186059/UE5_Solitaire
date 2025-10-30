#pragma once

#include "CoreMinimal.h"
#include "KKTweenHead.h"
using namespace KKTweenAPI;

class KKTween
{
public:
    class EaseFunc
    {
    public:
        static FVector2D easeLinear(FVector2D from, FVector2D to, float fTimePercent)
        {
            return from * (1 - fTimePercent) + to * fTimePercent;
        }

        static float easeLinear(float from, float to, float fTimePercent)
        {
            return from * (1 - fTimePercent) + to * fTimePercent;
        }

        static FVector easeLinear(FVector from, FVector to, float fTimePercent)
        {
            return from * (1 - fTimePercent) + to * fTimePercent;
        }
    private:
        EaseFunc() = delete;
        ~EaseFunc() = delete;
        EaseFunc(const EaseFunc&) = delete;
        EaseFunc& operator=(const EaseFunc&) = delete;
        EaseFunc(EaseFunc&&) = delete;
        EaseFunc& operator=(EaseFunc&&) = delete;
    };

public:
    static KKTweenItem* AddTween(float time, Action_Float_Delegate updateFunc = nullptr, ActionDelegate finishFunc = nullptr)
    {
        return AKKTweenMgr::GetSingleton()->AddTween(time, updateFunc, finishFunc);
    }

    static KKTweenItem* AddTween(UObject* obj, float time, Action_Float_Delegate updateFunc = nullptr, ActionDelegate finishFunc = nullptr)
    {
        return AKKTweenMgr::GetSingleton()->AddTween(obj, time, updateFunc, finishFunc);
    }

    static KKTweenItem* delayedCall(float time, ActionDelegate finishFunc = nullptr)
    {
        return AddTween(time, nullptr, finishFunc);
    }

    static KKTweenItem* delayedCall(UObject* obj, float time, ActionDelegate finishFunc = nullptr)
    {
        return AddTween(obj, time, nullptr, finishFunc);
    }

    static KKTweenItem* UMGMoveLocalRender(UWidget* go, FVector2D to, float fTime)
    {
        FVector InnerFrom = FVector(UMGHelper::GetRenderPos(go), 0);
        FVector InnerTo = FVector(to, 0);

        return KKTween::AddTween(
            go,
            time,
            [](float fPercent)
            {
                FVector targetPos = EaseFunc::easeLinear(InnerFrom, InnerTo, fPercent);
                UMGHelper::SetRenderPos(go, targetPos);
            });
    }

    //static KKTweenItem* UMGMoveLocalSlot(UWidget* go, FVector2D to, float fTime)
    //{
    //    FVector InnerFrom = FVector(UMGHelper::GetRenderPos(go), 0);
    //    FVector InnerTo = FVector(to, 0);

    //    return AddTween(
    //        go,
    //        time,
    //        [](float fPercent)
    //        {
    //            FVector2D beginPos = UMGHelper::GetRenderPos();

    //            FVector targetPos = EaseFunc::easeLinear(InnerFrom, InnerTo, fPercent);
    //            UMGHelper::SetRenderPos(go, targetPos);
    //        },
    //        []()
    //        {

    //        });
    //}
};

   