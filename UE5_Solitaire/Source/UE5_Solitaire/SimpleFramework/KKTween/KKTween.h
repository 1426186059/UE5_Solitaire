#pragma once

#include "CoreMinimal.h"
#include "KKTweenHead.h"
using namespace KKTweenAPI;

class KKTween
{
public:
    static KKTweenItem* AddTween(float time, Action_Float_Delegate updateFunc, ActionDelegate finishFunc)
    {
        return AKKTweenMgr::GetSingleton()->AddTween(time, updateFunc, finishFunc);
    }

    static KKTweenItem* AddTween(UObject* obj, float time, Action_Float_Delegate updateFunc, ActionDelegate finishFunc)
    {
        return AKKTweenMgr::GetSingleton()->AddTween(obj, time, updateFunc, finishFunc);
    }

    static KKTweenItem* delayedCall(float time, ActionDelegate finishFunc)
    {
        return AddTween(time, nullptr, finishFunc);
    }

    static KKTweenItem* delayedCall(UObject* obj, float time, ActionDelegate finishFunc)
    {
        return AddTween(obj, time, nullptr, finishFunc);
    }
};

   