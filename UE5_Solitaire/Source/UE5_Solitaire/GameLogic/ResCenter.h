// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKActorSingleton.h"
#include "PaperSprite.h"

#include "CardHandler.h"
#include "DataCenter.h"
#include "ThemeDataHandler.h"

class AResCenter : public AKKActorSingleton
{
public:
	static AResCenter* GetSingleton(bool bCreate = true)
	{
		return AKKActorSingleton::GetSingleton<AResCenter>(bCreate);
	}

	void Init();
	UPaperSprite* GetPokerSprite(int nPokerId);
};
