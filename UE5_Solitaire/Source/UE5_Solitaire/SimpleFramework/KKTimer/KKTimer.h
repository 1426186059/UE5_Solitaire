#pragma once

#include "CoreMinimal.h"
#include "KKFrameUpdateMgr.h"
#include "KKUETimer.h"
#include "KKFrameTimer.h"
#include "KKFixedTimer.h"
#include "KKFixedTimeMgr.h"

class KKTimer:public TSharedFromThis<KKTimer>
{
private:
	static TMap<uint64, TSharedPtr<KKTimer>> mKeepAliveDic;
	KKTimer() = default;
public:
	~KKTimer()
	{
		UE_LOG(LogTemp, Log, TEXT("~KKTimer Destroy"));
	}

	bool bUnScaled = false;
	int nLoopCount = 0;
	float fDuration = 0.0f;
	float time = 0.0f;
	bool running = false;
	TFunction<bool()> mDoFunc;
	TWeakObjectPtr<UObject> mBindObj;
	FDelegateHandle mTimerHandle;

	static TSharedPtr<KKTimer> New(UObject* bindObj, TFunction<bool()> func, float duration, int loop = 1, bool unscaled = false)
	{
		if (bindObj == nullptr)
		{
			bindObj = AKKFrameUpdateMgr::GetSingleton();
		}

		TSharedPtr<KKTimer> o = MakeShareable(new KKTimer());
		o->mBindObj = bindObj;
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
		mTimerHandle = AKKFrameUpdateMgr::GetSingleton()->GetEventList()->AddLambda([this](float DeltaTime) {
			this->Update(DeltaTime);
			});

		mKeepAliveDic.Add((uint64)this, this->AsShared());
	}

	void Stop()
	{
		this->running = false;
		AKKFrameUpdateMgr::GetSingleton()->GetEventList()->Remove(mTimerHandle);
		mKeepAliveDic.Remove((uint64)this);
	}

	void Update(float DeltaTime)
	{
		if (!this->running)
		{
			return;
		}

		if (mBindObj.Get() == nullptr)
		{
			this->Stop();
			return;
		}

		float delta = DeltaTime;
		this->time -= delta;

		if (this->time <= 0)
		{
			if (this->mDoFunc.IsSet())
			{
				if (!this->mDoFunc())
				{
					this->Stop();
					return;
				}
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