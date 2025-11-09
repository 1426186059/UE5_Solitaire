// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/MulticastDelegateBase.h"
#include "KKSingleton.h"

class UE5_SOLITAIRE_API KKEventMgr : public KKSingleton<KKEventMgr>
{
    friend class KKSingleton<KKEventMgr>;
public:
    DECLARE_MULTICAST_DELEGATE_OneParam(Action_voidPtr_Delegate, void*);

    Action_voidPtr_Delegate* GetEventList(int nEventId)
    {
        if (!mEventDic.Contains(nEventId))
        {
            Action_voidPtr_Delegate mEventList;
            mEventDic.Add(nEventId, mEventList);
        }
        return &mEventDic[nEventId];
    }
    
    void RemoveListener(int nEventId, FDelegateHandle Func)
    {
        if (mEventDic.Contains(nEventId))
        {
            mEventDic[nEventId].Remove(Func);
        }
    }

    void RemoveListener(int nEventId, FDelegateUserObjectConst mObj)
    {
        if (mEventDic.Contains(nEventId))
        {
            mEventDic[nEventId].RemoveAll(mObj);
        }
    }

    void RemoveListener(int nEventId)
    {
        if (mEventDic.Contains(nEventId))
        {
            mEventDic[nEventId].Clear();
            mEventDic.Remove(nEventId);
        }
    }

    void RemoveAllListener()
    {
        mEventDic.Empty();
    }

    void Broadcast(int nEventId, void* Param = nullptr)
    {
        if (mEventDic.Contains(nEventId))
        {
            mEventDic[nEventId].Broadcast(Param);
        }
    }

private:
    KKEventMgr() = default;
    ~KKEventMgr() = default;
    KKEventMgr(const KKEventMgr&) = delete;
    KKEventMgr& operator=(const KKEventMgr&) = delete;
    TMap<int32, Action_voidPtr_Delegate> mEventDic;
};
