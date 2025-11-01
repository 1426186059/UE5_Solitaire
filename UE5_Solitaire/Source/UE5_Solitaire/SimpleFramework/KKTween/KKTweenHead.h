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
    typedef TFunction<void(void)> ActionDelegate;
    typedef TFunction<void(float)> Action_Float_Delegate;

    class KKTweenByList;
    class KKTweenItem;
    class ObjectPool;

    class KKTweenItem
    {
        friend class ObjectPool;
    public:
        struct Handle
        {
            friend class KKTweenItem;
        private:
            int nId = -1;
            TWeakPtr<KKTweenItem> mInnerPtr;

            Handle(KKTweenItem* mItem)
            {
                this->mInnerPtr = TSharedPtr<KKTweenItem>(mItem);
                this->nId = mItem->nId;
            }
        public:
            Handle()
            {
               
            }

            bool IsValid()
            {
                return mInnerPtr.IsValid() && mInnerPtr.Pin()->nId == nId;
            }

            TWeakPtr<KKTweenItem> Get()
            {
                if (IsValid())
                {
                    return mInnerPtr;
                }
                return nullptr;
            }

            void AppendTween(Handle mOtherTween)
            {
                if (IsValid() && mOtherTween.IsValid())
                {
                    Get().Pin()->AppendTween(mOtherTween.Get().Pin().Get());
                }
            }

            void AppendTween(TWeakPtr<KKTweenItem> mOtherTween)
            {
                if (IsValid() && mOtherTween.IsValid())
                {
                    Get().Pin()->AppendTween(mOtherTween.Pin().Get());
                }
            }

            void Cancel()
            {
                if (IsValid())
                {
                    Get().Pin()->cancel();
                }

                mInnerPtr.Reset();
                nId = -1;
            }
        };

        TWeakObjectPtr<UObject> bindObj;
        bool toggle;
        float delay;
        float time;
        float sumTime;
        int32 nLoopCount;
        int32 nLoopPingTong;
        int32 nId;
        Action_Float_Delegate updateFunc;
        ActionDelegate finishFunc;

        FVector From;
        FVector To;

    private:
        void OnPoolPop()
        {
            
        }

        void OnPoolRecycle()
        {
            nId++;
            ensureMsgf(nId <= MAX_int32, TEXT("KKTweenItem Error"));
            Reset();
        }

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
            updateFunc.Reset();
            finishFunc.Reset();

            nLoopCount = 0;
            nLoopPingTong = 0;
        }

    public:
        Handle Build()
        {
            return Handle(this);
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

        void AppendTween(KKTweenItem* mItem)
        {
            float mTweenSumTime = this->delay + this->sumTime;
            mItem->delay += mTweenSumTime;
        }

        KKTweenItem* SetOnUpdateFunc(Action_Float_Delegate func)
        {
            this->updateFunc = func;
            return this;
        }

        KKTweenItem* SetOnCompleteFunc(ActionDelegate func)
        {
            this->finishFunc = func;
            return this;
        }
    };

    class ObjectPool
    {
    private:
        TArray<TSharedPtr<KKTweenItem>> mObjectPool;
        int nMaxCapacity = 0;
    public:
        ObjectPool()
        {
            SetMaxCapacity(1500);
        }

        void SetMaxCapacity(int nCapacity = 1)
        {
            this->nMaxCapacity = nCapacity;
        }

        TSharedPtr<KKTweenItem> Pop()
        {
            if (mObjectPool.Num() > 0)
            {
                auto mItem = mObjectPool.Pop();
                mItem->OnPoolPop();
                return TSharedPtr<KKTweenItem>(mItem);
            }
            else
            {
                return TSharedPtr<KKTweenItem>(new KKTweenItem());
            }
        }

        void recycle(TSharedPtr<KKTweenItem> t)
        {
            if (mObjectPool.Num() >= nMaxCapacity)
            {
                t.Reset();
            }
            else
            {
                t->OnPoolRecycle();
                mObjectPool.Add(t);
            }
        }
    };

    class KKTweenByList
    {
    private:
        ObjectPool mItemPool;
        TArray<TSharedPtr<KKTweenItem>> mTweenT;
        AKKTweenMgr* defaultBindObj;
    public:
        KKTweenByList(AKKTweenMgr* mDefaultBindObj);

        void Update(float DeltaTime);
        void SetMaxTweenCount(int nCount);
        TWeakPtr<KKTweenItem> AddTween(
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
    void SetMaxTweenCount(int nCount);
    TWeakPtr<KKTweenAPI::KKTweenItem> AddTween(UObject* obj, float time, KKTweenAPI::Action_Float_Delegate updateFunc = nullptr, KKTweenAPI::ActionDelegate finishFunc = nullptr);
    TWeakPtr<KKTweenAPI::KKTweenItem> delayedCall(UObject* obj, float time, KKTweenAPI::ActionDelegate finishFunc = nullptr);
public:
    //DECLARE_MULTICAST_DELEGATE(ActionDelegate);
    //DECLARE_MULTICAST_DELEGATE_OneParam(Action_Float_Delegate, float);
    //TFunction<void()> ActionDelegate; //UE5 提供的方法指针
    //using Action_Float_Delegate = void(*)(float); //C++ 定义方法指针
    //TFunction<void(float)> Action_Float_Delegate; //UE5 提供的方法指针
    //typedef void (*ActionDelegate)(); //C语言定义方法指针
    //typedef void (*Action_Float_Delegate)(float); //C语言定义方法指针
};
