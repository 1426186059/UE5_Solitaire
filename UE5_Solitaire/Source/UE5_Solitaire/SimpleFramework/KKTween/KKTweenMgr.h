#pragma once

#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "KKTween.h"
#include "KKTweenByList.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKTweenMgr.generated.h"

namespace KKTween
{
    UCLASS()
    class UE5_SOLITAIRE_API AKKTweenMgr : public AKKActorSingleton
    {
        GENERATED_BODY()

    protected:
        AKKTweenMgr();
        virtual void BeginPlay() override;
        virtual void EndPlay(EEndPlayReason::Type Reason) override;
        virtual void Tick(float DeltaTime) override;
    public:
        static AKKTweenMgr* GetSingleton(bool bCreate = true)
        {
            return AKKActorSingleton::GetSingleton<AKKTweenMgr>(bCreate);
        }

    private:
        KKTween::KKTweenByList* mManager;

    public:
        void Update(float DeltaTime);
        //KKTween::KKTweenItem* AddTween(float time, KKTween::Action_Float_Delegate updateFunc = nullptr, KKTween::ActionDelegate finishFunc = nullptr);
        //KKTween::KKTweenItem* AddTween(UObject* obj, float time, KKTween::Action_Float_Delegate updateFunc = nullptr, KKTween::ActionDelegate finishFunc = nullptr);
        //KKTween::KKTweenItem* delayedCall(float time, KKTween::ActionDelegate finishFunc = nullptr);
        //KKTween::KKTweenItem* delayedCall(UObject* obj, float time, KKTween::ActionDelegate finishFunc = nullptr);
    public:
        //DECLARE_MULTICAST_DELEGATE(ActionDelegate);
        //DECLARE_MULTICAST_DELEGATE_OneParam(Action_Float_Delegate, float);
        //TFunction<void()> ActionDelegate; //UE5 提供的方法指针
        //using Action_Float_Delegate = void(*)(float); //C++ 定义方法指针
        //TFunction<void(float)> Action_Float_Delegate; //UE5 提供的方法指针
        //typedef void (*ActionDelegate)(); //C语言定义方法指针
        //typedef void (*Action_Float_Delegate)(float); //C语言定义方法指针
    };
}
