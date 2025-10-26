// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"
#include "PaperSprite.h"

#include "CardHandler.h"
#include "DataCenter.h"

class ResCenter : public KKSingleton<ResCenter>
{
public:
	void Init();
	UPaperSprite* GetPokerSprite(int nPokerId);
};
