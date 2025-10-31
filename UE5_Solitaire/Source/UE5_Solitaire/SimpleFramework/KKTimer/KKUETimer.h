#pragma once

#include "CoreMinimal.h"

class KKUETimer
{
	bool bUnScaled = false;
	int nLoopCount = 0;
	float fDuration = 0.0f;
	float time = 0.0f;
	bool running = false;
	TFunction<void()> mDoFunc;
	FTimerHandle  mTimerHandle;
	TWeakObjectPtr<UObject> mBindObj;

public:
	static KKUETimer* New(UObject* bindObj, TFunction<void()> func, float duration, int loop = 1, bool unscaled = false)
	{
		if (bindObj == nullptr)
		{
			bindObj = AKKFrameUpdateMgr::GetSingleton();
		}

		auto o = new KKUETimer();
		o->mDoFunc = func;
		o->fDuration = duration;
		o->time = duration;
		o->nLoopCount = loop;
		o->bUnScaled = unscaled;
		o->running = false;
		o->mBindObj = bindObj;
		return o;
	}

	void Start()
	{
		this->running = true;
		UEHelper::GetKKWorld()->GetTimerManager().SetTimer(
			mTimerHandle,
			[this]()
			{
				this->Update();
			},
			0.0f,
			true);
	}

	void Reset(TFunction<void()> func, float duration, int loop = 1, bool unscaled = false)
	{
		this->fDuration = duration;
		this->nLoopCount = loop;
		this->bUnScaled = unscaled;
		this->mDoFunc = func;
		this->time = duration;
	}

	void Stop()
	{
		this->running = false;
		UEHelper::GetKKWorld()->GetTimerManager().ClearTimer(mTimerHandle);
	}

	void Update()
	{
		float DeltaTime = UEHelper::GetKKWorld()->GetTimerManager().GetTimerRate(mTimerHandle);
		if (!this->running)
		{
			return;
		}

		if (mBindObj.Get() == nullptr)
		{
			this->Stop();
			return;
		}

		this->time -= DeltaTime;
		if (this->time <= 0)
		{
			if (this->mDoFunc.IsSet())
			{
				this->mDoFunc();
			}

			if (this->nLoopCount > 0)
			{
				this->nLoopCount--;
				this->time += this->fDuration;
			}

			if (this->nLoopCount == 0)
			{
				this->Stop();
			}
			else if (this->nLoopCount < 0)
			{
				this->time += this->fDuration;
			}
		}

	}
};
