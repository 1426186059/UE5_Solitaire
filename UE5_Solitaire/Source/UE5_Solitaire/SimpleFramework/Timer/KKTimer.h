#include "CoreMinimal.h"
#include "KKTimeMgr.h"

class KKTimer
{
	bool unscaled = false;
	int loop = 0;
	float duration = 0.0f;
	float time = 0.0f;
	bool running = false;
	TFunction<void()> func;

public:
	static KKTimer* New(TFunction<void()> func, float duration, int loop = 1, bool unscaled = false)
	{
		auto o = new KKTimer();
		o->func = func;
		o->duration = duration;
		o->time = duration;
		o->loop = loop;
		o->unscaled = unscaled;
		o->running = false;
		return o;
	}

	void Start()
	{
		this.running = true;
		KKTimeMgr::GetSingleton()->AddListener(this.Update);
	}

	void Reset(Action func, float duration, int loop = 1, bool unscaled = false)
	{
		this->duration = duration;
		this->loop = loop;
		this->unscaled = unscaled;
		this->func = func;
		this->time = duration;
	}

	void Stop()
	{
		this.running = false;
		KKTimeMgr::GetSingleton()->RemoveListener(this.Update);
	}

	void Update()
	{
		if (!this->running)
		{
			return;
		}

		float delta = UEHelper::GetTime_DeltaTime(this->unscaled);
		this->time -= delta;

		if (this->time <= 0)
		{
			this->func();

			if (this->loop > 0)
			{
				this->loop--;
				this->time += this->duration;
			}

			if (this->loop == 0)
			{
				this->Stop();
			}
			else if (this->loop < 0)
			{
				this->time += this->duration;
			}
		}
	}
};
			