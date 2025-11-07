#pragma once

#include "CoreMinimal.h"

#define N_DEFAULT_MAX_CAPACITY 1024

template<typename T>
class KKObjectPool
{
private:
	TArray<TSharedPtr<T>> mObjectPool = {};
	TMap<uint64, TSharedPtr<T>> mAllDic = {};
	int nMaxCapacity;

private:
	TSharedPtr<T> InnerCreateItem()
	{
		TSharedPtr<T> t = TSharedPtr(new T());
		mAllDic[(int64)t.Get()] = t;
		return t;
	}
public:
	void Init(int nInitCount = 0, int maxCapacity = 0)
	{
		for (int i = 0; i < nInitCount; i++)
		{
			mObjectPool.Add(InnerCreateItem());
		}

		if (maxCapacity <= 0)
		{
			this->SetMaxCapacity(N_DEFAULT_MAX_CAPACITY);
		}
		else
		{
			this->SetMaxCapacity(maxCapacity);
		}
	}

	void SetMaxCapacity(int nCapacity)
	{
		this->nMaxCapacity = nCapacity;
	}

	TSharedPtr<T> Pop()
	{
		if (mObjectPool.Num() > 0)
		{
			return mObjectPool.pop();
		}
		else
		{
			return InnerCreateItem();
		}
	}

	void Recycle(TSharedPtr<T> t)
	{
		if (t->IsValid())
		{
			uint64 nKey = (uint64)t.Get();
			if (this->nMaxCapacity > 0 && mObjectPool.Num() >= this->nMaxCapacity)
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
		mObjectPool.Empty();
		mAllDic.Empty();
	}

	int Count()
	{
		return mObjectPool.Num();
	}
};
