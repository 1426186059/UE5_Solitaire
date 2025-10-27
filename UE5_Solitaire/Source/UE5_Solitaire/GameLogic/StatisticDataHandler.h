// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "GameData.h"
#include "DataCenter.h"
#include "GameConst.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKEventMgr.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"

class StatisticDataHandler :public KKSingleton<StatisticDataHandler>
{
	
public:
	UStatisticData* data;

public:
	void Init();
};
