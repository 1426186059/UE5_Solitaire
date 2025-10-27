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

class ThemeDataHandler :public KKSingleton<ThemeDataHandler>
{
	
public:
	UThemeData* data;

public:
	void Init();
	
	bool hasThemeid(int32 id);
	bool hasBgid(int32 id);
	bool hasFrontid(int32 id);
	bool hasBackid(int32 id);
	void addThemeid(int32 id);
	void addbgid(int32 id);
	void addfrontid(int32 id);
	void addbackid(int32 id);

	void InitDefaultTheme();
	void InitDefaultThemeElement();
	void UnLockNewTheme(csv_theme::RowData mThemeConfig);
	void UseNewTheme(csv_theme::RowData mThemeConfig);
	void UseNewThemeBg(int32 Id);
	void UseNewThemeFront(int32 Id);
	void UseNewThemeBack(int32 Id);
	void RefreshThemePai();
	bool orThemeUnLockToUnLockMe(csv_themeitem::RowData mTargetConfig);
	FString GetBgThemeName(csv_themeitem::RowData mTargetConfig);
	//int32 GetBgUnLock_ForStageRemainWinCount(csv_stagereward::RowData mTargetConfig);
	int32 GetThemePaiId_ForBgId(int32 Internalid);
	csv_theme::RowData* GetUsedThemeConfig();
	csv_theme::RowData* GetThemeConfigById(int32 nThemeId);
	int32 GetThemeElements_internalid_byId(int32 id);
	int32 GetThemeElements_Id_byInternalidAndType(int32 Internalid, int32 type);
	FString GetThemeThumbPath(int32 nId, int32 nType);
	FString GetGamePoolBg_AtlasSpriteName(int32 nPaiId);
	FString GetGameResultBg_AtlasSpriteName(int32 nPaiId);
	FString GetGameLongBg_AtlasSpriteName(int32 nPaiId);
};
