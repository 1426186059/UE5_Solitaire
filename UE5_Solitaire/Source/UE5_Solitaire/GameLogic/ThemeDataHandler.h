// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "GameData.h"
#include "DataCenter.h"
#include "GameConst.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKRandomTool.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"

class ThemeDataHandler :public KKSingleton<ThemeDataHandler>
{
	
public:
	UThemeData* data;

public:
	void Init();
};
