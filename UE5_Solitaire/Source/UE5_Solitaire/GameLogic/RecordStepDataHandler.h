// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "DataCenter.h"
#include "GameConst.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKRandomTool.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"

class RecordStepDataHandler :public KKSingleton<RecordStepDataHandler>
{
public:
	void Init();
	void InitData();
	FRecordStepData* GetData();
	FRecordStepData_OpStepItem GetOpStepItemDefaultData();
	void SetDbDataWithMeta();
	void InitStepRecordFromOther(const FRecordStepData& mmmData);
	void InitStepRecord(int32 nGameMode, const TArray<int>& mInitSendCardList);
	void AddUndoCount(int nCount);
	void ResetUndoCountTo0();
	void AddStepRecord(const FRecordStepData_OpStepItem& mOpStepItemData);
	std::tuple<int32, FRecordStepData_OpStepItem*> GetNowStepRecord();
	bool orCanUndo();
	bool orCanForward();
	FString GetPosTypeName(SolitairePokerPosType nPosType);
	void PrintOp(const FRecordStepData_OpStepItem& mOpStepItemData);
	void PrintAllOp();

	void AddMoveCount();
	void AddScore(int32 nCount);
	void AddTime(int32 nCount);
	void SetLoseWin(bool bWin);
	bool orGameEnd();
};
