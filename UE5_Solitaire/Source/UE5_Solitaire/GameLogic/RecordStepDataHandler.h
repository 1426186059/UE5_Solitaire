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

class RecordStepDataHandler :public KKSingleton<RecordStepDataHandler>
{
	
public:
	URecordStepData* data;

public:
	void Init();
	URecordStepData* GetDefaultData();
	URecordStepData_OpStepItem* GetOpStepItemDefaultData();
	void SetDbDataWithMeta();
	void InitStepRecordFromOther(URecordStepData* mmmData);
	void InitStepRecord(int32 nGameMode, TArray<int> mInitSendCardList);
	void AddUndoCount(int nCount);
	void ResetUndoCountTo0();
	void AddStepRecord(URecordStepData_OpStepItem* mOpStepItemData);
	std::tuple<int32, URecordStepData_OpStepItem*> GetNowStepRecord();
	bool orCanUndo();
	bool orCanForward();
	FString GetPosTypeName(SolitairePokerPosType nPosType);
	void PrintOp(URecordStepData_OpStepItem* mOpStepItemData);
	void PrintAllOp();

	void AddMoveCount();
	void AddScore(int32 nCount);
	void AddTime(int32 nCount);
	void SetLoseWin(bool bWin);
	bool orGameEnd();
};
