#pragma once

#include "CoreMinimal.h"

namespace KKTween
{
    typedef void (*ActionDelegate)(); //C语言定义方法指针
    typedef void (*Action_Float_Delegate)(float); //C语言定义方法指针
    
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

}

   