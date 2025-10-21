// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "GameData.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

typedef void (*InitFinishFunc)();

class DataCenter:public KKSingleton<DataCenter>
{
public:
	const FString DATA_SLOT_NAME = "SolitaireData";
	const int32 DATA_USER_INDEX = 0;

	UGameData* data;
	InitFinishFunc mInitFinishFunc;
	void Init(InitFinishFunc func = NULL);

	void LoadData();
	void SaveData();

	void OnLoadDataComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGameInstance);
	void OnSaveDataComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
