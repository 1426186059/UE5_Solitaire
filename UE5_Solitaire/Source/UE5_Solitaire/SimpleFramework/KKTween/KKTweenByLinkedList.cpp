#include "KKTweenHead.h"

namespace KKTweenAPI
{
    KKTweenByLinkedList::KKTweenByLinkedList(AKKTweenMgr* mDefaultBindObj)
    {
        this->defaultBindObj = mDefaultBindObj;
    }

    void KKTweenByLinkedList::Update(float DeltaTime)
    {
        auto mNode = mTweenT.GetHead();
        while (mNode != nullptr)
        {
            auto mItem = mNode->GetValue();
            if (mItem->toggle == false || mItem->bindObj == nullptr)
            {
                mTweenT.RemoveNode(mItem->mEntry, false);
                mItemPool.recycle(mItem);
                continue;
            }

            if (mItem->delay > 0.0f)
            {
                mItem->delay -= DeltaTime;
                mNode = mNode->GetNextNode();
                continue;
            }

            if (mItem->nLoopPingTong > 0)
            {
                if (mItem->nLoopPingTong == 2)
                {
                    mItem->time -= DeltaTime;
                }
                else
                {
                    mItem->time += DeltaTime;
                }

                mItem->time = FMath::Clamp(mItem->time, 0.0, mItem->sumTime);
                float fTimePercent = mItem->time / mItem->sumTime;
                if (mItem->updateFunc.IsSet())
                {
                    mItem->updateFunc(fTimePercent);
                }

                if (mItem->nLoopPingTong == 2)
                {
                    if (fTimePercent <= 0)
                    {
                        if (mItem->finishFunc.IsSet())
                        {
                            mItem->finishFunc();
                        }

                        mItem->nLoopPingTong = 1;
                        if (mItem->nLoopCount > 0)
                        {
                            mItem->nLoopCount--;

                            if (mItem->nLoopCount <= 0)
                            {
                                mTweenT.RemoveNode(mItem->mEntry, false);
                                mItemPool.recycle(mItem);
                                continue;
                            }
                        }
                    }
                }
                else
                {
                    if (fTimePercent >= 1.0f)
                    {
                        mItem->nLoopPingTong = 2;
                    }
                }

            }
            else
            {
                mItem->time += DeltaTime;
                mItem->time = FMath::Clamp(mItem->time, 0, mItem->sumTime);
                float fTimePercent = mItem->time / mItem->sumTime;

                if (mItem->updateFunc.IsSet())
                {
                    mItem->updateFunc(fTimePercent);
                }

                if (fTimePercent >= 1.0f)
                {
                    if (mItem->finishFunc.IsSet())
                    {
                        mItem->finishFunc();
                    }

                    if (mItem->nLoopCount == -1)
                    {
                        mItem->time = 0;
                    }
                    else
                    {
                        mItem->nLoopCount--;
                        mItem->time = 0;

                        if (mItem->nLoopCount <= 0)
                        {
                            mTweenT.RemoveNode(mItem->mEntry, false);
                            mItemPool.recycle(mItem);
                            continue;
                        }
                    }
                }
            }

            mNode = mNode->GetNextNode();
        }
    }

    void KKTweenByLinkedList::SetMaxTweenCount(int nCount)
    {
        mItemPool.SetMaxCapacity(nCount);
    }

    TSharedPtr<KKTweenItem> KKTweenByLinkedList::AddTween(
        UObject* obj,
        float time,
        Action_Float_Delegate updateFunc,
        ActionDelegate finishFunc)
    {
        check(obj);
        check(time >= 0);

        if (obj == nullptr)
        {
            obj = defaultBindObj;
        }

        TSharedPtr<KKTweenItem> mItem = mItemPool.Pop();
        mItem->bindObj = obj;
        mItem->toggle = true;
        mItem->time = 0;
        mItem->sumTime = time;
        mItem->updateFunc = updateFunc;
        mItem->finishFunc = finishFunc;
        mTweenT.AddTail(mItem->GetEntry());
        return mItem;
    }
}
