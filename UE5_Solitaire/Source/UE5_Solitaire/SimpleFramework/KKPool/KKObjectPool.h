#pragma once

#include "CoreMinimal.h"

#define N_DEFAULT_MAX_CAPACITY 1024

template<typename T>
class KKObjectPool
{
private:
	TArray<TSharedPtr<T>> mObjectPool = {};
	TMap<uint64, TSharedPtr<T>> mAllDic = {};


private:
	TSharedPtr<T> InnerCreateItem()
	{
		TSharedPtr<T> t = TSharedPtr(new T());
		mAllDic[(int64)t.Get()] = t;
		return t;
	}
public:
	void Init(int nInitCount = 0, int nMaxCapacity = 0)
	{
		for (int i = 0; i < nInitCount; i++)
		{
			mObjectPool.Add(InnerCreateItem());
		}

		if (nMaxCapacity <= 0)
		{
			this->SetMaxCapacity(N_DEFAULT_MAX_CAPACITY);
		}
		else
		{
			this->SetMaxCapacity(nMaxCapacity);
		}
	}

	void SetMaxCapacity(int nCapacity)
	{
		this->nMaxCapacity = nCapacity;
	}

	TSharedPtr<T> Pop()
	{
		T mItem = null;
		if (mObjectPool.Count > 0)
		{
			mItem = mObjectPool.pop();
		}
		else
		{
			mItem = InnerCreateItem();
		}

		mUsedObjectPool.push(mItem);
		return mItem;
	}

	void Recycle(TSharedPtr<T> t)
	{
		if (t->IsValid())
		{
			uint64 nKey = (uint64)t.Get();
			if (mObjectPool.Num() >= this->nMaxCapacity)
			{
				mAllDic.Remove(nKey);
				t.Reset();
			}
			else
			{
				if (mAllDic.Contains(nKey))
				{
					mObjectPool.Push(t);
				}
				else
				{
					ensureMsgf(false, TEXT("KKObjectPool Recycle Error"));
				}
			}
		}
	}

	void RecycleAll()
	{
		this->mObjectPool.RemoveAll();
		for (auto [k, v] : this->mAllDic)
		{
			this->mObjectPool.Add(v);
		}
	}

	void Release()
	{
		mObjectPool.Clear();
		mUsedDic.Clear();
	}

	int Count()
	{
		return mObjectPool.Num();
	}

	TSharedPtr<T> GetSharePtr(T* oriPtr)
	{
		uint64 nKey = (uint64)oriPtr;
		if (mAllDic.Contains(nKey))
		{
			return mAllDic[nKey];
		}
		else
		{
			return nullptr;
		}
	}
};
