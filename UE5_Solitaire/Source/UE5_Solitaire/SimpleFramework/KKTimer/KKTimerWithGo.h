#pragma once

#include "KKTimeMgr.h"
#include "CoreMinimal.h"

class TimerWithGo
{
public:
    bool unscaled = false;
    int loop = 0;
    float duration = 0.0f;
    float time = 0.0f;
    bool running = false;
    TFunction<void()> mDoFunc;
    TWeakObjectPtr<UObject> bindObj;
    FDelegateHandle mTimerHandle;

    static TimerWithGo* New(UObject* bindObj, TFunction<void()> func, float duration, int loop = 1, bool unscaled = false)
    {
        auto o = new TimerWithGo();
        o->bindObj = bindObj;
        o->mDoFunc = func;
        o->duration = duration;
        o->time = duration;
        o->loop = loop;
        o->unscaled = unscaled;
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

        if (bindObj.Get() == nullptr)
        {
            this->Stop();
            return;
        }

        float delta = UEHelper::GetTime_DeltaTime(this->unscaled);
        this->time -= delta;

        if (this->time <= 0)
        {
            if (this->mDoFunc)
            {
                this->mDoFunc();
            }

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
