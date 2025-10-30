#pragma once

#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKTweenHead.generated.h"

namespace KKTweenAPI
{
    //typedef void (*ActionDelegate)(); //C语言定义方法指针
    //typedef void (*Action_Float_Delegate)(float); //C语言定义方法指针

    // 修改你的 typedef
    typedef TFunction<void()> ActionDelegate;
    typedef TFunction<void(float)> Action_Float_Delegate;

    class KKTweenByList;
    class KKTweenItem;
    class ObjectPool;

    class KKTweenItem
    {
    public:
        TWeakObjectPtr<UObject> bindObj;
        bool toggle;
        float delay;
        float time;
        float sumTime;
        int nLoopCount;
        int nLoopPingTong;
        Action_Float_Delegate updateFunc;
        ActionDelegate finishFunc;

        FVector From;
        FVector To;

    public:
        KKTweenItem()
        {
            Reset();
        }

        void Reset()
        {
            bindObj = nullptr;
            toggle = false;

            delay = 0.0;
            time = 0.0;
            sumTime = 0.0;
            updateFunc = nullptr;
            finishFunc = nullptr;

            nLoopCount = 0;
            nLoopPingTong = 0;
        }

        KKTweenItem* cancel()
        {
            toggle = false;
            return this;
        }

        KKTweenItem* SetDelay(float fTime)
        {
            this->delay = fTime;
            return this;
        }

        KKTweenItem* SetLoop(int nCount = -1)
        {
            this->nLoopCount = nCount;
            return this;
        }

        KKTweenItem* SetLoopPingPong(int nCount = -1)
        {
            this->nLoopCount = nCount;
            nLoopPingTong = 1;
            return this;
        }

        KKTweenItem* AppendTween(KKTweenItem* mItem)
        {
            float mTweenSumTime = this->delay + this->sumTime;
            mItem->delay += mTweenSumTime;
            return this;
        }

        KKTweenItem* SetOnComplete(ActionDelegate func)
        {
            this->finishFunc = func;
            return this;
        }
    };

    class ObjectPool
    {
    private:
        TArray<KKTweenItem*> mObjectPool;
    public:
        KKTweenItem* Pop()
        {
            if (mObjectPool.Num() > 0)
            {
                return mObjectPool.Pop();
            }
            else
            {
                return new KKTweenItem();
            }
        }

        void recycle(KKTweenItem* t)
        {
            t->Reset();
            mObjectPool.Add(t);
        }
    };

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
};


UCLASS()
class UE5_SOLITAIRE_API AKKTweenMgr : public AKKActorSingleton
{
    GENERATED_BODY()

protected:
    AKKTweenMgr();
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type Reason) override;
public:
    virtual void Tick(float DeltaTime) override;
public:
    static AKKTweenMgr* GetSingleton(bool bCreate = true)
    {
        return AKKActorSingleton::GetActorSingleton<AKKTweenMgr>(bCreate);
    }

private:
    KKTweenAPI::KKTweenByList* mManager;

public:
    void Update(float DeltaTime);
    KKTweenAPI::KKTweenItem* AddTween(float time, KKTweenAPI::Action_Float_Delegate updateFunc = nullptr, KKTweenAPI::ActionDelegate finishFunc = nullptr);
    KKTweenAPI::KKTweenItem* AddTween(UObject* obj, float time, KKTweenAPI::Action_Float_Delegate updateFunc = nullptr, KKTweenAPI::ActionDelegate finishFunc = nullptr);
    KKTweenAPI::KKTweenItem* delayedCall(float time, KKTweenAPI::ActionDelegate finishFunc = nullptr);
    KKTweenAPI::KKTweenItem* delayedCall(UObject* obj, float time, KKTweenAPI::ActionDelegate finishFunc = nullptr);
public:
    //DECLARE_MULTICAST_DELEGATE(ActionDelegate);
    //DECLARE_MULTICAST_DELEGATE_OneParam(Action_Float_Delegate, float);
    //TFunction<void()> ActionDelegate; //UE5 提供的方法指针
    //using Action_Float_Delegate = void(*)(float); //C++ 定义方法指针
    //TFunction<void(float)> Action_Float_Delegate; //UE5 提供的方法指针
    //typedef void (*ActionDelegate)(); //C语言定义方法指针
    //typedef void (*Action_Float_Delegate)(float); //C语言定义方法指针
};
