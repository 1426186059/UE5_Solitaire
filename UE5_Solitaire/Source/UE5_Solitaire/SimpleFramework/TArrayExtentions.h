// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class TArrayExtentions
{
public:
    template<typename T>
    static T RemoveLast(TArray<T>& mList)
    {
        return mList.Pop();
    }

    template<typename T>
    static T Remove(TArray<T>& mList, int nIndex)
    {
        T t = mList[nIndex];
        mList.RemoveAt(nIndex);
        return t;
    }

    template<typename T>
    static T RandomRemove(TArray<T>& mList, int nBeginIndex = 0, int nEndIndex = -1)
    {
        ensureMsgf(nBeginIndex >= 0 && nBeginIndex < mList.Num(), TEXT("nBeginIndex: %d"), nBeginIndex);
        ensureMsgf(nEndIndex >= 0 && nEndIndex < mList.Num(), TEXT("nEndIndex: %d"), nEndIndex);

        if (nEndIndex == -1)
        {
            nEndIndex = mList.Num() - 1;
        }

        int nRemoveIndex = KKRandomTool::RandomInt(nBeginIndex, nEndIndex);
        return TArrayExtentions::Remove(mList, nRemoveIndex);
    }

private:
    // 禁止拷贝和移动（单例不应被复制）
    TArrayExtentions() = delete;
    ~TArrayExtentions() = delete;
    TArrayExtentions(const TArrayExtentions&) = delete;
    TArrayExtentions& operator=(const TArrayExtentions&) = delete;
    TArrayExtentions(TArrayExtentions&&) = delete;
    TArrayExtentions& operator=(TArrayExtentions&&) = delete;
};
