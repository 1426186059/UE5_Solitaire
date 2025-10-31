#include "UE5_Solitaire/SimpleFramework/UEHelper.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKTimeMgr.generated.h"

UCLASS()
class UE5_SOLITAIRE_API AKKTimeMgr : public AKKActorSingleton
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;

private:
    TArray<TFunction<void()>> mapUpdateFunc;
public:
    static KKTimeMgr* GetSingleton(bool bCreate = true)
    {
        return AKKActorSingleton::GetActorSingleton<KKTimeMgr>(bCreate);
    }

    void Update()
    {
        int nUpdateCount = mapUpdateFunc.Num();
        for (int i = 0; i < nUpdateCount; i++)
        {
            if (i < mapUpdateFunc.Count)
            {
                mapUpdateFunc[i]();
            }
            else
            {
                break;
            }
        }
    }

    void AddListener(TFunction<void()> func)
    {
        if (mapUpdateFunc.IndexOf(func) == -1)
        {
            mapUpdateFunc.Add(func);
        }
    }

    void RemoveListener(TFunction<void()> func)
    {
        this.mapUpdateFunc.Remove(func);
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

    void Init(float fInternalTime = 1.0f)
    {
        this->fInternalTime = fInternalTime;
        this->Reset();
    }

    void Reset()
    {
        this->fLastUpdateTime = UGameplayStatics::GetTimeSeconds(UEHelper::GetKKWorld());
    }

    bool orTimeOut()
    {
        float fNowTime = UGameplayStatics::GetTimeSeconds(UEHelper::GetKKWorld());
        if ((fNowTime - fLastUpdateTime) > fInternalTime)
        {
            this->Reset();
            return true;
        }

        return false;
    }

    bool orTimeOutWithSpecialTime(float fInternalTime)
    {
        float fNowTime = UGameplayStatics::GetTimeSeconds(UEHelper::GetKKWorld());
        if (fNowTime - fLastUpdateTime > fInternalTime)
        {
            this->Reset();
            return true;
        }

        return false;
    }
};

