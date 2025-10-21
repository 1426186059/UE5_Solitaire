// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/Function.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "HAL/CriticalSection.h"

#include "KKSingleton.h"

class UE5_SOLITAIRE_API KKEventMgr : public KKSingleton
{
public:
    template<typename TMsg>
    void Broadcast(const TMsg& Msg)
    {
        FScopeLock Lock(&Mutex);
        if (auto* Listeners = MsgMap.Find(GetTypeHash<TMsg>()))
        {
            // 拷贝一份，防止回调里移除监听导致迭代器失效
            TArray<FGenericListener> Copy = *Listeners;
            for (auto& Func : Copy)
            {
                Func.Invoke(&Msg);
            }
        }
    }
    
    template<typename TMsg>
    FDelegateHandle Listen(void* Owner, TFunction<void(const TMsg&)> Func)
    {
        FScopeLock Lock(&Mutex);
        const uint32 Key = GetTypeHash<TMsg>();
        FGenericListener Listener;
        Listener.Owner = Owner;
        Listener.Handle = FDelegateHandle::GenerateNewHandle();
        Listener.Callback = [Func](const void* Raw) { Func(*static_cast<const TMsg*>(Raw)); };
        MsgMap.FindOrAdd(Key).Add(MoveTemp(Listener));
        return Listener.Handle;
    }
    
    void UnlistenAll(void* Owner)
    {
        FScopeLock Lock(&Mutex);
        for (auto& KV : MsgMap)
        {
            KV.Value.RemoveAll([Owner](const FGenericListener& L) { return L.Owner == Owner; });
        }
    }
    
    void Unlisten(FDelegateHandle Handle)
    {
        FScopeLock Lock(&Mutex);
        for (auto& KV : MsgMap)
        {
            KV.Value.RemoveAll([Handle](const FGenericListener& L) { return L.Handle == Handle; });
        }
    }

private:
    KKEventMgr() = default;
    ~KKEventMgr() = default;
    KKEventMgr(const KKEventMgr&) = delete;
    KKEventMgr& operator=(const KKEventMgr&) = delete;

    /* 内部泛型 Listener */
    struct FGenericListener
    {
        void* Owner = nullptr;
        FDelegateHandle Handle;
        TFunction<void(const void*)> Callback;

        void Invoke(const void* Msg) const { Callback(Msg); }
    };

    /* 根据类型生成唯一 Key */
    template<typename T>
    static uint32 GetTypeHash()
    {
        static char PlaceHolder = 0;
        return GetTypeHash(&PlaceHolder);
    }

    TMap<uint32, TArray<FGenericListener>> MsgMap;
    FCriticalSection Mutex;
};
