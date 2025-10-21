// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/MulticastDelegateBase.h"
#include "KKSingleton.h"

DECLARE_MULTICAST_DELEGATE_OneParam(mTMulticastDelegate, void*);

class UE5_SOLITAIRE_API KKEventMgr : public KKSingleton<KKEventMgr>
{
public:
    FDelegateHandle AddListener(int nEventId, mTMulticastDelegate::FDelegate Func)
    {
        if (!mEventDic.Contains(nEventId))
        {
            mTMulticastDelegate mEventList;
            mEventDic.Add(nEventId, mEventList);
        }
        return mEventDic[nEventId].Add(Func);
    }
    
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

    TMap<int32, mTMulticastDelegate> mEventDic;
};
