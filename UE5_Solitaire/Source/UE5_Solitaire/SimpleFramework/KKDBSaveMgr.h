#pragma once

#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKDBSaveMgr.generated.h"

UCLASS()
class AKKDBSaveMgr : public AKKActorSingleton
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(ActionDelegate, void);

	static AKKDBSaveMgr* GetSingleton(bool bCreate = true)
	{
		return AKKActorSingleton::GetActorSingleton<AKKDBSaveMgr>(bCreate);
	}

private:
	ActionDelegate mOnSaveDBEventList;
	void Fire()
	{
		mOnSaveDBEventList.Broadcast();
	}
protected:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		Super::EndPlay(EndPlayReason);

#if WITH_EDITOR
		this->Fire();
#endif
	}

	virtual void Tick(float DeltaTime) override
	{
		Super::Tick(DeltaTime);
	}
public:
	void Init()
	{
#if !WITH_EDITOR
		FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddLambda([this]()
			{
				UE_LOG(LogTemp, Log, TEXT("==>AKKDBSaveMgr OnApplicationPause true"));
				this->Fire();
			});

		FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddLambda([this]()
			{
				UE_LOG(LogTemp, Log, TEXT("==>AKKDBSaveMgr OnApplicationPause false"));
			});
#endif
	}

	ActionDelegate* GetEventList()
	{
		return &mOnSaveDBEventList;
	}
};
