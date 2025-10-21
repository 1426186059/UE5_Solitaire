// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/MulticastDelegateBase.h"
#include "KKSingleton.h"

DECLARE_MULTICAST_DELEGATE_OneParam(Action_voidPtr_Delegate, void*);

class UE5_SOLITAIRE_API KKEventMgr : public KKSingleton<KKEventMgr>
{
    friend class KKSingleton<KKEventMgr>;
public:
    Action_voidPtr_Delegate* GetEventList(int nEventId)
    {
        if (!mEventDic.Contains(nEventId))
        {
            Action_voidPtr_Delegate mEventList;
            mEventDic.Add(nEventId, mEventList);
        }
        return &mEventDic[nEventId];
    }

    //FDelegateHandle AddListener(int nEventId, void* Func)
    //{
    //    Action_voidPtr_Delegate mList = GetEventList(nEventId)
    //    return mList.AddRaw(nullptr, Func);
    //}

    //FDelegateHandle AddListener(int nEventId, Action_voidPtr_Delegate::FDelegate Func)
    //{
    //    if (!mEventDic.Contains(nEventId))
    //    {
    //        Action_voidPtr_Delegate mEventList;
    //        mEventDic.Add(nEventId, mEventList);
    //    }
    //    return mEventDic[nEventId].AddRaw(Func);
    //}
    
    void RemoveListener(int nEventId, FDelegateHandle Func)
    {
        if (mEventDic.Contains(nEventId))
        {
            mEventDic[nEventId].Remove(Func);
        }
    }

    void Broadcast(int nEventId, void* Param)
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
