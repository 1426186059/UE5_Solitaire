#pragma once

#include "UE5_Solitaire/SimpleFramework/UEHelper.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKTimeMgr.generated.h"

UCLASS()
class UE5_SOLITAIRE_API AKKTimeMgr : public AKKActorSingleton
{
    GENERATED_BODY()
    DECLARE_MULTICAST_DELEGATE_OneParam(UpdateDelegate, float);
public:
    AKKTimeMgr();
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;

private:
    UpdateDelegate mUpdateDelegate;
    void Update(float DeltaTime)
    {
        this->mUpdateDelegate.Broadcast(DeltaTime);
    }
public:
    static AKKTimeMgr* GetSingleton(bool bCreate = true)
    {
        return AKKActorSingleton::GetActorSingleton<AKKTimeMgr>(bCreate);
    }

    UpdateDelegate* GetEventList()
    {
        return &this->mUpdateDelegate;
    }
};

class KKTimeOutGenerator
{
    float fLastUpdateTime = 0;
    float fInternalTime = 0;

public:
    static KKTimeOutGenerator* New(float fInternalTime)
    {
        auto temp = new KKTimeOutGenerator();
        temp->Init(fInternalTime);
        return temp;
    }

    void Init(float paramInternalTime = 1.0f)
    {
        this->fInternalTime = paramInternalTime;
        this->Reset();
    }

    void Reset()
    {
        this->fLastUpdateTime = UEHelper::GetTime_Time();
    }

    bool orTimeOut()
    {
        float fNowTime = UEHelper::GetTime_Time();
        if ((fNowTime - fLastUpdateTime) > fInternalTime)
        {
            this->Reset();
            return true;
        }

        return false;
    }

    bool orTimeOutWithSpecialTime(float paramInternalTime)
    {
        float fNowTime = UEHelper::GetTime_Time();
        if (fNowTime - fLastUpdateTime > paramInternalTime)
        {
            this->Reset();
            return true;
        }

        return false;
    }
};

