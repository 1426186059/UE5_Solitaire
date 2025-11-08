#pragma once

#include "CoreMinimal.h"
#include "../KKUI/KKUI.h"
#include "../KKTimer/KKTimer.h"

#define N_DEFAULT_MAX_CAPACITY 1024

template<typename T = UUserWidget>
class KKWidgetPool
{
private:
    TMap<uint64, TWeakObjectPtr<T>> mKeepAliveDic;
    TArray<TObjectPtr<T>> mPool = {};
    TSubclassOf<UUserWidget> mWidgetClass;
    UPanelWidget* mItemParent;
    int nMaxCapicity = -1;

private:
    TObjectPtr<T> InnerCreateItem()
    {
        T* mItem = AKKUIMgr::GetSingleton()->KKCreateWidget<T>(this->mWidgetClass);
        UMGHelper::SetParent(mItem, this->mItemParent);
        UMGHelper::SetSlotAnchor(mItem, FAnchors(0.5));
        UMGHelper::SetSlotAlignment(mItem, FVector2D(0.5));
        UMGHelper::SetSlotSize(mItem, FVector2D(0, 0));
        mItem->SetVisibility(ESlateVisibility::Hidden);

        auto mItemPtr = TWeakObjectPtr<T>(mItem);
        mKeepAliveDic.Add((uint64)mItem, mItemPtr);
        return mItem;
    }
    
public:
    KKWidgetPool()
    {
        static_assert(TIsDerivedFrom<T, UUserWidget>::Value, "T must be an UWidget derived class");
    }

    //由于Widget 具有父Widget, 所以不会被 GC 掉，这个 ItemParent 是必须的。
    void Init(TSubclassOf<UUserWidget> mClass, UPanelWidget* ItemParent, int nInitCount = 0)
    {
        this->nMaxCapicity = N_DEFAULT_MAX_CAPACITY;
        this->mWidgetClass = mClass;
        this->mItemParent = ItemParent;
        this->preLoadObj(nInitCount);
    }

    TObjectPtr<T> Pop()
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
        return mItem;
    }
    
    void Recycle(TObjectPtr<T> mItem)
    {
        if (mItem == nullptr)
        {
            return;
        }

        uint64 nKey = (uint64)mItem.Get();
        if (mKeepAliveDic.Contains(nKey))
        {
            if (this->nMaxCapicity > 0 && this->mPool.Num() >= this->nMaxCapicity)
            {
                mKeepAliveDic.Remove(nKey);
                UMGHelper::DestroyWidget(mItem);
            }
            else
            {
                UMGHelper::SetParent(mItem, this->mItemParent);
                mItem->SetVisibility(ESlateVisibility::Hidden);
                this->mPool.Add(mItem);
            }
        }
        else
        {
            ensureMsgf(false, TEXT("recyleObj Error"));
            UMGHelper::DestroyWidget(mItem);
        }
    }

    void SetMaxCapacity(int nMaxCapacity)
    {
        this->nMaxCapicity = nMaxCapacity;
    }

    int GetSumCount()
    {
        return this->mKeepAliveDic.Num();
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
                        }
                        return false;
                    }
                    else
                    {
                        this->mPool.Add(this->InnerCreateItem());
                    }
                }

                //UE_LOG(LogTemp, Log, TEXT("KKWidgetPool preLoadObj: %d"), this->GetSumCount());
                return true;
            }, 1 / 60.0f, nFrameCount);
        mTimer->Start();
    }

};
