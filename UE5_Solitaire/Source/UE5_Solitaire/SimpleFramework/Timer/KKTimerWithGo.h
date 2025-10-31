#include "CoreMinimal.h"
#include "KKTimeMgr.h"

class TimerWithGo
{
public:
    bool unscaled = false;
    int loop = 0;
    float duration = 0.0f;
    float time = 0.0f;
    bool running = false;
    TFunction<void()> func;
    TWeakObjectPtr<UObject> bindObj;

    static TimerWithGo* New(UObject* bindObj, TFunction<void()> func, float duration, int loop = 1, bool unscaled = false)
    {
        auto o = new TimerWithGo();
        o->bindObj = bindObj;
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
        this->running = true;
        AKKTimeMgr::GetSingleton()->GetEventList()->AddUFunction(this, TimerWithGo::Update);
    }

    void Stop()
    {
        this.running = false;
        AKKTimeMgr::GetSingleton()->GetEventList()->RemoveAll(this);
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
