using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

//Object 池子
public class ObjectPool<T> where T : class, new()
{
	readonly TSArray<T> mObjectPool = new TSArray<T>();
	readonly TSArray<T> mUsedObjectPool = new TSArray<T>();

	public int Count()
	{
		return mObjectPool.Count;
	}

	public void initObj(int count)
    {
		for(int i = 0; i < count; i++)
        {
			T t = new T();
			mObjectPool.push(t);
        }
    }

	public T popObj()
	{
		T mItem = null;
		if (mObjectPool.Count > 0)
		{
			mItem = mObjectPool.pop();
		}
		else
		{
			mItem = new T();
		}
		
		if(GameLauncher.Instance.m_PrintLog)
		{
			PrintTool.Assert(!mUsedObjectPool.Contains(mItem));
		}

		mUsedObjectPool.push(mItem);
		return mItem;
	}

	public void recycleAllObj()
	{
		for(int i = 0; i < mUsedObjectPool.Count; i++)
        {
			mObjectPool.push(mUsedObjectPool[i]);
        }
		mUsedObjectPool.Clear();
	}

	public void release()
	{
		mObjectPool.Clear();
	}
}

	