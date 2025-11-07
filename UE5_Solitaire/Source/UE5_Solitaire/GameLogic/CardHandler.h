// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "DataCenter.h"
#include "GameConst.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"
#include "UE5_Solitaire/SimpleFramework/KKRandomTool.h"
#include "UE5_Solitaire/SimpleFramework/KKTimeTool.h"
#include "UE5_Solitaire/SimpleFramework/TArrayExtentions.h"
#include "UE5_Solitaire/CSV/CSVConfigMgr.h"
#include "UE5_Solitaire/DataTable/DTMgr.h"

class CardHandler :public KKSingleton<CardHandler>
{
private:
	TMap<int32, int32> ConfusingDic;
public:
	void Init();
	void InitConfusingDic();

	TArray<int> GetInitCards_ForNormalMode();
	TArray<int> GetInitCards_ForRankMode();
	TArray<int> GetInitCards_ForChallengeMode();
	TArray<int> GetInitCards_Random_ForEasy();
	TArray<int> GetInitCards_ExcelRandom(int nDifficultLayer, int nGameLevel);
	TArray<int> GetInitCards_Random();
	int GetPokerId(int nDigitId, int nColorType);
	int GetDigital(int nPokerId);
	int GetSubDigital(int nPokerId);
	std::tuple<int, int> GetTwoDigital(int nPokerId);
	int GetColor(int nPokerId);
	int GetExcelToLocalPokerId(int nExcelNum);
	std::tuple<bool, TArray<int>> GetExcelTablePokerId_ForHalfWay(FDT_jianhuan_vita* configItem);
	std::tuple<bool, TArray<int>> GetExcelTablePokerId(FDT_jianhuan_vita* configItem);
	void reverseTable(TArray<int>& t, int nBeginIndex, int nEndIndex);
	bool CheckCardListError(const TArray<int>& mCardList);
	FDT_jianhuan_vita* GetVitaConfigItem2(FString sid);
	std::tuple<bool, TArray<int>> GetExcelTablePokerId(TArray<int> tablePokerId);
};
