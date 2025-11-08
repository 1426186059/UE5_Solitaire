// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "DataCenter.h"

class ThemeDataHandler : public KKSingleton<ThemeDataHandler>
{
public:
	void Init();

	void InitData();
	FThemeData* GetData();
	
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
	void UnLockNewTheme(FDT_theme* mThemeConfig);
	void UseNewTheme(FDT_theme* mThemeConfig);
	void UseNewThemeBg(int32 Id);
	void UseNewThemeFront(int32 Id);
	void UseNewThemeBack(int32 Id);
	void RefreshThemePai();
	bool orThemeUnLockToUnLockMe(FDT_themeitem* mTargetConfig);
	FString GetBgThemeName(FDT_themeitem* mTargetConfig);
	int32 GetBgUnLock_ForStageRemainWinCount(FDT_stagereward* mTargetConfig);
	int32 GetThemePaiId_ForBgId(int32 Internalid);
	FDT_theme* GetUsedThemeConfig();
	FDT_theme* GetThemeConfigById(int32 nThemeId);
	int32 GetThemeElements_internalid_byId(int32 id);
	int32 GetThemeElements_Id_byInternalidAndType(int32 Internalid, int32 type);
	FString GetThemeThumbPath(int32 nId, int32 nType);
	FString GetGamePoolBg_AtlasSpriteName(int32 nPaiId);
	FString GetGameResultBg_AtlasSpriteName(int32 nPaiId);
	FString GetGameLongBg_AtlasSpriteName(int32 nPaiId);
};
