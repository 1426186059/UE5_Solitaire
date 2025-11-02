#pragma once

#include "CoreMinimal.h"
#include "../KKUI/UMGHelper.h"
#include "../KKTimer/KKTimer.h"

template<typename T>
class KKWidgetPool
{
private:
    TArray<T*> mPool = {};
    TArray<T*> usedArray = {};

    TSubclassOf<UWidget> mWidgetClass;
    UWidget* ItemParent;
    int nMaxCapicity = -1;

private:
    T* InnerCreateItem()
    {
        T* mItem = CreateWidget<T>(this, this->mWidgetClass);
        UMGHelper::SetParent(mItem, this->ItemParent);
        UMGHelper::SetSlotAnchor(mItem, FAnchors(0.5));
        UMGHelper::SetSlotAlignment(mItem, FVector2D(0.5));
        UMGHelper::SetSlotSize(mItem, FVector2D(0, 0));
        mItem->SetVisibility(ESlateVisibility::Hidden);
        mPool.Add(mItem);
        return mItem;
    }
    
public:
    void Init(TSubclassOf<UWidget> mClass, UWidget* ItemParent = nullptr, int nInitCount = 0)
    {
        static_assert(TIsDerivedFrom<T, UWidget>::Value, "T must be an UWidget derived class");

        this->mWidgetClass = mClass;
        this->ItemParent = ItemParent;
        this->preLoadObj(nInitCount);
    }
    
    void recycleObj(T* mItem)
    {
        UMGHelper::SetParent(mItem, this->ItemParent);

        int nIndex = this->usedArray.IndexOf(mItem);
        ensureMsgf(nIndex >= 0, TEXT("recyleObj 000 Error: %d"), nIndex);
        this->usedArray.RemoveAt(nIndex);
        nIndex = this.usedArray.IndexOf(mItem);
        ensureMsgf(nIndex == -1, TEXT("recyleObj 111 Error"));

        mItem->SetVisibility(ESlateVisibility::Hidden);
        this->mPool.Add(mItem);
    }

    T* popObj()
    {
        T* mItem = nullptr;
        if (this->mPool.length > 0)
        {
            mItem = this->mPool.Pop();
        }
        else
        {
            mItem = this->InnerCreateItem();
        }

        mItem->SetVisibility(ESlateVisibility::Visible);
        this.usedArray.Add(mItem);

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
        return this->usedArray.Num() + this->mPool.Count;
    }

    void SetItemParent(UWidget* ItemParent)
    {
        this->ItemParent = ItemParent;
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

    void preLoadObj(int nFrameCount, int nCount, TFunction<void()> mFinishFunc = null)
    {
        int nCreateCountSingle = FMath::CeilToInt(nCount / (float)nFrameCount);
        Timer mTimer = Timer.New(
            []()
            {
                for (int j = 0; j < nCreateCountSingle; j++)
                {
                    if (this->GetSumCount() >= nCount)
                    {
                        if (mFinishFunc.IsSet())
                        {
                            mFinishFunc();
                            mFinishFunc = nullptr;
                        }
                        break;
                    }
                    else
                    {
                        this.this->mPool.push(this.InnerCreateItem());
                    }
                }
            }, 1 / 60f, nFrameCount);
        mTimer.Start();
    }
};
