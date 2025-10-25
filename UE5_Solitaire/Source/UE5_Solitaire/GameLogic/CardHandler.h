// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "GameData.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

class CardHandler :public KKSingleton<CardHandler>
{
public:
	void Init();

	void LoadData();
	void SaveData();
};
