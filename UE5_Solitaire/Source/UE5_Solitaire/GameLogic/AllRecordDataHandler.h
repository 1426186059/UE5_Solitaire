// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "GameData.h"
#include "DataCenter.h"
#include "GameConst.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKEventMgr.h"
#include "UE5_Solitaire/SimpleFramework/KKTimeTool.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"

class AllRecordDataHandler :public KKSingleton<AllRecordDataHandler>
{
	
public:
	static const FString GameRecordDirPath;
	static const FString GameRecordFileIndexName;

	UAllRecordData* data;
public:
	void Init();
	void SaveDb();
	void SaveGameRecordToDirectory();
	URecordStepData* GetDataByFileItem(UAllRecordData_fileNameItem* fileNameItem);
	URecordStepData* RemoveAndGetDataByFileItem(UAllRecordData_fileNameItem* fileNameItem);
	URecordStepData* RemoveAndGetLastDifferentGameModeRecordTable(int32 nNowGameMode);
	void ClearAllTripGame();
	URecordStepData* RemoveAndGetTripHalfGame(); //²Ð¾Ö;

};
