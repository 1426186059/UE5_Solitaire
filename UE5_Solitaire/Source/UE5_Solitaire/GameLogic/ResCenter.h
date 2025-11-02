// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKPool/KKWidgetPool.h"

#include "CardHandler.h"
#include "DataCenter.h"
#include "ThemeDataHandler.h"
#include "ResCenter.generated.h"

class UPokerAnimationItemW;

UCLASS()
class AResCenter : public AKKActorSingleton
{
	GENERATED_BODY()
public:


public:
	static AResCenter* GetSingleton(bool bCreate = true)
	{
		return AKKActorSingleton::GetActorSingleton<AResCenter>(bCreate);
	}

	void Init();
	UPaperSprite* GetPokerSprite(int nPokerId);
};
