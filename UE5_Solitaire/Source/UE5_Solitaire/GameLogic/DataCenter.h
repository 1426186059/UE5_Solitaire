// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "UE5_Solitaire/SaveGame/GameSaveGame.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

typedef void (*InitFinishFunc)();

class DataCenter:public KKSingleton<DataCenter>
{
public:
	static const FString DATA_SLOT_NAME;
	static const int32 DATA_USER_INDEX;

	UGameData* data;
	InitFinishFunc mInitFinishFunc;
	void Init(InitFinishFunc func = NULL);

	UGameData* GetData();
	void LoadData(bool bSync = true);
	void SaveData(bool bSync = true);

	void OnLoadDataComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGameInstance);
	void OnSaveDataComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	void AddMagicWandCount(int32 nAddCount);
	void AddTotalGameCount();
	void AddTotalWinGameCount();
	void AddCoinCount(int32 nCointCount);
	void SetDrawCount(int32 nDrawCount);
	void AddGameLevel();
	void AddNomalModeTotalWinCount();
	void AddDifficultLayerWinResult(bool bWin);
	void SetLanguageName(FString langName);
	void SetUIStype(int32 nUIStyle);
	void SetSoundOpen(bool bTrue);
	void SetClickToMove(bool bTrue);
	void SetAutoHint(bool bTrue);
	void SetAutoComplete(bool bTrue);
	void SetFastGame(bool bTrue);
	void SetIQMode(bool bTrue);
	void AddIQValue(int32 nIQValue);
	void UpdateMusic(int32 nMusicIndex);
	void SetLeftHandMode(bool bLeftHandMode);

};
