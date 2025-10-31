#pragma once

#include "CoreMinimal.h"
#include "KKTimeMgr.h"
#include "KKTimerWithGo.h"
#include "KKFrameTimer.h"
#include "KKFixedTimer.h"
#include "KKFixedTimeMgr.h"

class KKTimer
{
	bool bUnScaled = false;
	int nLoopCount = 0;
	float fDuration = 0.0f;
	float time = 0.0f;
	bool running = false;
	TFunction<void()> mDoFunc;
	FDelegateHandle mTimerHandle;
public:
	static KKTimer* New(TFunction<void()> func, float duration, int loop = 1, bool unscaled = false)
	{
		auto o = new KKTimer();
		o->mDoFunc = func;
		o->fDuration = duration;
		o->time = duration;
		o->nLoopCount = loop;
		o->bUnScaled = unscaled;
		o->running = false;
		return o;
	}

	void Start()
	{
		this->running = true;
		mTimerHandle = AKKTimeMgr::GetSingleton()->GetEventList()->AddLambda([this](float DeltaTime) {
			this->Update(DeltaTime);
			});
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
		AKKTimeMgr::GetSingleton()->GetEventList()->Remove(mTimerHandle);
	}

	void Update(float DeltaTime)
	{
		if (!this->running)
		{
			return;
		}

		float delta = UEHelper::GetTime_DeltaTime(this->bUnScaled);
		this->time -= delta;

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
			