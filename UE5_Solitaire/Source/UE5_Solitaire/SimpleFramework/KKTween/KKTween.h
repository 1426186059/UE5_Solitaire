#pragma once

#include "KKTweenHead.h"
#include "UE5_Solitaire/SimpleFramework/KKUI/UMGHelper.h"
#include "CoreMinimal.h"

using namespace KKTweenAPI;

class KKTween
{
public:
    class EaseFunc
    {
    public:
        template<typename T>
        static T easeLinear(T from, T to, float fTimePercent)
        {
            return from * (1 - fTimePercent) + to * fTimePercent;
        }

        template<typename T>
        static T easeInOutQuad(T from, T to, float timePercent)
        {
            timePercent = timePercent * 2.0;
            to = to - from;
            if (timePercent < 1.0)
            {
                return to / 2 * timePercent * timePercent + from;
            }

            timePercent -= 1.0;
            return -to / 2 * (timePercent * (timePercent - 2) - 1) + from;
        }

        template<typename T>
        static T easeInOutSine(T from, T to, float timePercent)
        {
            to = to - from;
            return -to / 2 * (FMath::Cos(PI * timePercent) - 1) + from;
        }

        template<typename T>
        static T easeInOutQuart(T from, T to, float timePercent)
        {
            timePercent = timePercent * 2.0;
            to = to - from;
            if (timePercent < 1.0)
            {
                return to / 2.0 * timePercent * timePercent * timePercent * timePercent + from;
            }

            timePercent = timePercent - 2;
            return -to / 2.0 * (timePercent * timePercent * timePercent * timePercent - 2) + from;
        }
        
        template<typename T>
        static T easeOutQuad(T from, T to, float timePercent)
        {
            T diff = to - from;
            float val = -timePercent * (timePercent - 2);
            return diff * val + from;
        }

        template<typename T>
        static T easeInQuad(T from, T to, float timePercent)
        {
            T diff = to - from;
            float val = timePercent * timePercent;
            return diff * val + from;
        }

        template<typename T>
        static T easeOutSine(T from, T to, float timePercent)
        {
            T diff = to - from;
            float val = FMath::Sin(timePercent * PI / 2);
            return from + diff * val;
        }

        template<typename T>
        static T easeInSine(T from, T to, float timePercent)
        {
            T diff = to - from;
            float val = -FMath::Cos(timePercent * PI / 2);
            return from + diff + diff * val;
        }

        //static FVector tweenOnCurve(FVector from, FVector to, float timePercent, animationCurve)
        //{
        //    return from + (to - from) * animationCurve:Evaluate(timePercent);
        //}

          //local LTBezierPath = CS.LTBezierPath(to);
  /*      function TweenCommonFunc : tweenOnBezierPath(val, mLTBezierPath)
            return mLTBezierPath : point(val);
        end*/

    private:
        EaseFunc() = delete;
        ~EaseFunc() = delete;
        EaseFunc(const EaseFunc&) = delete;
        EaseFunc& operator=(const EaseFunc&) = delete;
        EaseFunc(EaseFunc&&) = delete;
        EaseFunc& operator=(EaseFunc&&) = delete;
    };

public:
    static void SetMaxTweenCount(int nCount)
    {
        return AKKTweenMgr::GetSingleton()->SetMaxTweenCount(nCount);
    }

    static KKTweenItem* AddTween(float time, Action_Float_Delegate updateFunc = nullptr, ActionDelegate finishFunc = nullptr)
    {
        return AKKTweenMgr::GetSingleton()->AddTween(nullptr, time, updateFunc, finishFunc);
    }

    static KKTweenItem* AddTween(UObject* obj, float time, Action_Float_Delegate updateFunc = nullptr, ActionDelegate finishFunc = nullptr)
    {
        return AKKTweenMgr::GetSingleton()->AddTween(obj, time, updateFunc, finishFunc);
    }
    
    static KKTweenItem* delayedCall(float time, ActionDelegate finishFunc = nullptr)
    {
        return AddTween(nullptr, time, nullptr, finishFunc);
    }

    static KKTweenItem* delayedCall(UObject* obj, float time, ActionDelegate finishFunc = nullptr)
    {
        return AddTween(obj, time, nullptr, finishFunc);
    }

    static KKTweenItem* UMG_MoveLocal_Render(UWidget* target, FVector2D to, float time)
    {
        FVector2D InnerFrom = UMGHelper::GetRenderPos(target);
        FVector2D InnerTo = to;
        return AddTween(target, time,
            [=](float fPercent)
            {
                FVector2D targetPos = EaseFunc::easeLinear(InnerFrom, InnerTo, fPercent);
                UMGHelper::SetRenderPos(target, targetPos);
            });
    }

    static KKTweenItem* UMG_MoveLocal_Slot(UWidget* target, FVector2D to, float time)
    {
        FVector2D InnerFrom = UMGHelper::GetSlotPos(target);
        FVector2D InnerTo = to;
        return AddTween(target, time,
            [=](float fPercent)
            {
                FVector2D targetPos = EaseFunc::easeLinear(InnerFrom, InnerTo, fPercent);
                UMGHelper::SetRenderPos(target, targetPos);
            });
    }
};

   