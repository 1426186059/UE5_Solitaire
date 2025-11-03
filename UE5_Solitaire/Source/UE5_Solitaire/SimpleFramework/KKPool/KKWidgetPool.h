#pragma once

#include "CoreMinimal.h"
#include "../KKUI/KKUIMgr.h"
#include "../KKTimer/KKTimer.h"

template<typename T = UUserWidget>
class KKWidgetPool
{
private:
    TArray<T*> mPool = {};
    TArray<T*> usedArray = {};

    TSubclassOf<UUserWidget> mWidgetClass;
    UPanelWidget* mItemParent;
    int nMaxCapicity = -1;

private:
    T* InnerCreateItem()
    {
        T* mItem = AKKUIMgr::GetSingleton()->CreateKKWidget<T>(this->mWidgetClass);
        UMGHelper::SetParent(mItem, this->mItemParent);
        UMGHelper::SetSlotAnchor(mItem, FAnchors(0.5));
        UMGHelper::SetSlotAlignment(mItem, FVector2D(0.5));
        UMGHelper::SetSlotSize(mItem, FVector2D(0, 0));
        mItem->SetVisibility(ESlateVisibility::Hidden);
        mPool.Add(mItem);
        return mItem;
    }
    
public:
    KKWidgetPool()
    {
        static_assert(TIsDerivedFrom<T, UUserWidget>::Value, "T must be an UWidget derived class");
    }

    void Init(TSubclassOf<UUserWidget> mClass, UPanelWidget* ItemParent = nullptr, int nInitCount = 0)
    {
        this->mWidgetClass = mClass;
        this->mItemParent = ItemParent;
        this->preLoadObj(nInitCount);
    }
    
    void recycleObj(T* mItem)
    {
        UMGHelper::SetParent(mItem, this->mItemParent);

        int nIndex = this->usedArray.IndexOfByKey(mItem);
        ensureMsgf(nIndex >= 0, TEXT("recyleObj 000 Error: %d"), nIndex);
        this->usedArray.RemoveAt(nIndex);
        nIndex = this->usedArray.IndexOfByKey(mItem);
        ensureMsgf(nIndex == -1, TEXT("recyleObj 111 Error"));

        mItem->SetVisibility(ESlateVisibility::Hidden);
        this->mPool.Add(mItem);
    }

    T* popObj()
    {
        T* mItem = nullptr;
        if (this->mPool.Num() > 0)
        {
            mItem = this->mPool.Pop();
        }
        else
        {
            mItem = this->InnerCreateItem();
        }

        mItem->SetVisibility(ESlateVisibility::Visible);
        this->usedArray.Add(mItem);

        if (this->nMaxCapicity > 0 && this->usedArray.Num() + this->mPool.Num() > this->nMaxCapicity)
        {
            UE_LOG(LogTemp, Error, TEXT("Capicity Error: %d"), this.nMaxCapicity);
        }
        return mItem;
    }

    void SetMaxCapacity(int nMaxCapacity)
    {
        this.nMaxCapicity = nMaxCapacity;
    }

    int GetSumCount()
    {
        return this->usedArray.Num() + this->mPool.Num();
    }

    void SetItemParent(UPanelWidget* ItemParent)
    {
        this->mItemParent = ItemParent;
    }

    void preLoadObj(int nMaxCount)
    {
        int nNowCount = this->GetSumCount();
        for (int j = nNowCount; j < nMaxCount; j++)
        {
            this->mPool.Add(this->InnerCreateItem());
        }
    }

    //public IEnumerator preLoadObj_Co(int nFrameCount, int nCount, Action finishFunc = null)
    //{
    //    Action mFinishFunc = finishFunc;
    //    int nCreateCountSingle = Mathf.CeilToInt(nCount / nFrameCount);

    //    while(this.GetSumCount() < nCount)
    //    {
    //        for (int j = 0; j < nCreateCountSingle; j++)
    //        {
    //            if (this.GetSumCount() >= nCount)
    //            {
    //                if (mFinishFunc != null)
    //                {
    //                    mFinishFunc();
    //                    mFinishFunc = null;
    //                }
    //                break;
    //            }
    //            this->mPool.push(this.InnerCreateItem());
    //        }
    //        yield return null;
    //    }
    //}

    void preLoadObj(int nFrameCount, int nCount, TFunction<void()> mFinishFunc = nullptr)
    {
        int nCreateCountSingle = FMath::CeilToInt(nCount / (float)nFrameCount);
        auto mTimer = KKTimer::New(nullptr,
            [=, this]()
            {
                for (int j = 0; j < nCreateCountSingle; j++)
                {
                    if (this->GetSumCount() >= nCount)
                    {
                        if (mFinishFunc.IsSet())
                        {
                            mFinishFunc();
                            mFinishFunc;
                        }
                        return false;
                    }
                    else
                    {
                        this->mPool.Add(this->InnerCreateItem());
                    }
                }

                UE_LOG(LogTemp, Log, TEXT("KKWidgetPool preLoadObj: %d"), this->GetSumCount());
                return true;
            }, 1 / 60.0f, nFrameCount);
        mTimer->Start();
    }

};
