// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "DataCenter.h"
#include "GameConst.h"
#include "RecordStepDataHandler.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKEventMgr.h"
#include "UE5_Solitaire/SimpleFramework/KKTimeTool.h"
#include "UE5_Solitaire/SimpleFramework/TArrayExtentions.h"
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
	FRecordStepData* GetDataByFileItem(FAllRecordData_fileNameItem* fileNameItem);
	FRecordStepData* RemoveAndGetDataByFileItem(FAllRecordData_fileNameItem* fileNameItem);
	FRecordStepData* RemoveAndGetLastDifferentGameModeRecordTable(int32 nNowGameMode);
	void ClearAllTripGame();
	FRecordStepData* RemoveAndGetTripHalfGame(); //²Ð¾Ö;

};
