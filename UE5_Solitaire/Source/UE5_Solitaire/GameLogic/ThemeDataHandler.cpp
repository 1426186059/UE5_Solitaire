#include "ThemeDataHandler.h"

void ThemeDataHandler::Init()
{
	this->InitDefaultTheme();
	this->InitDefaultThemeElement();
}

void ThemeDataHandler::InitData()
{
	DataCenter::GetSingleton()->GetData()->ThemeData = {};
}

FThemeData* ThemeDataHandler::GetData()
{
	return &DataCenter::GetSingleton()->GetData()->ThemeData;
}

bool ThemeDataHandler::hasThemeid(int32 id)
{
	return this->GetData()->array_themeids.Contains(id);
}

bool ThemeDataHandler::hasBgid(int32 id)
{
	return this->GetData()->array_bgids.Contains(id);
}

bool ThemeDataHandler::hasFrontid(int32 id)
{
	return this->GetData()->array_frontids.Contains(id);
}

bool ThemeDataHandler::hasBackid(int32 id)
{
	return this->GetData()->array_backids.Contains(id);
}

void ThemeDataHandler::addThemeid(int32 id)
{
	if (!this->hasThemeid(id))
	{
		this->GetData()->array_themeids.Add(id);
	}
}

void ThemeDataHandler::addbgid(int32 id)
{
	if (!this->hasBgid(id))
	{
		this->GetData()->array_bgids.Add(id);
	}
}

void ThemeDataHandler::addfrontid(int32 id)
{
	if (!this->hasFrontid(id))
	{
		this->GetData()->array_frontids.Add(id);
	}
}

void ThemeDataHandler::addbackid(int32 id)
{
	if (!this->hasBackid(id))
	{
		this->GetData()->array_backids.Add(id);
	}
}

void ThemeDataHandler::InitDefaultTheme()
{
	if (this->GetData()->themeBgId <= 0)
	{
		auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_theme>()->GetTable();
		auto& mThemeConfig = (*mTable)[4];
		this->UnLockNewTheme(mThemeConfig);
		this->UseNewTheme(mThemeConfig);
	}
}

void ThemeDataHandler::InitDefaultThemeElement()
{
	auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_theme>()->GetTable();
	for (auto& v : *mTable)
	{
		auto& mConfig = v;
		if (this->hasThemeid(mConfig.id))
		{
			this->addbgid(this->GetThemeElements_internalid_byId(mConfig.table));
			this->addfrontid(this->GetThemeElements_internalid_byId(mConfig.front));
			this->addbackid(this->GetThemeElements_internalid_byId(mConfig.back));
		}
	}

	this->RefreshThemePai();
}

void ThemeDataHandler::UnLockNewTheme(const FDT_theme& mThemeConfig)
{
	this->addThemeid(mThemeConfig.id);
	this->InitDefaultThemeElement();
}


void ThemeDataHandler::UseNewTheme(const FDT_theme& mThemeConfig)
{
	this->GetData()->themeBgId = this->GetThemeElements_internalid_byId(mThemeConfig.table);
	this->GetData()->themeBackId = this->GetThemeElements_internalid_byId(mThemeConfig.back);
	this->GetData()->themeZhengId = this->GetThemeElements_internalid_byId(mThemeConfig.front);
	this->RefreshThemePai();
	KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdatePokerAtlas)->Broadcast(nullptr);
}

void ThemeDataHandler::UseNewThemeBg(int32 Id)
{
	this->GetData()->themeBgId = Id;
	this->RefreshThemePai();
	KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdatePokerAtlas)->Broadcast(nullptr);
}

void ThemeDataHandler::UseNewThemeFront(int32 Id)
{
	this->GetData()->themeZhengId = Id;
	KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdatePokerAtlas)->Broadcast(nullptr);
}

void ThemeDataHandler::UseNewThemeBack(int32 Id)
{
	this->GetData()->themeBackId = Id;
	KKEventMgr::GetSingleton()->GetEventList(GameConst::EventId_UpdatePokerAtlas)->Broadcast(nullptr);
}

void ThemeDataHandler::RefreshThemePai()
{
	int32 themeBgId = this->GetData()->themeBgId;
	if (themeBgId > 0)
	{
		this->GetData()->themePaiId = this->GetThemePaiId_ForBgId(themeBgId);
	}
}

//-- 是否主题中的元素，比如桌面Bg，牌面，牌背，跟随主题解锁
bool ThemeDataHandler::orThemeUnLockToUnLockMe(const FDT_themeitem& mTargetConfig)
{
	ensure(mTargetConfig.itemunlock == 4);
	auto mThemeConfig = CSVConfigMgr::GetSingleton()->GetCSV<csv_theme>()->GetTable();
	for (auto& v : *mThemeConfig)
	{
		auto& mConfig = v;
		if (mConfig.table == mTargetConfig.id)
		{
			return this->hasThemeid(mConfig.id);
		}
		else if (mConfig.back == mTargetConfig.id)
		{
			return this->hasThemeid(mConfig.id);
		}
		else if (mConfig.front == mTargetConfig.id)
		{
			return this->hasThemeid(mConfig.id);
		}
	}
	return false;
}

FString ThemeDataHandler::GetBgThemeName(const FDT_themeitem& mTargetConfig)
{
	ensure(mTargetConfig.itemunlock == 4);
	auto mThemeConfig = CSVConfigMgr::GetSingleton()->GetCSV<csv_theme>()->GetTable();
	for (auto& v : *mThemeConfig)
	{
		auto& mConfig = v;
		if (mConfig.table == mTargetConfig.id)
		{
			//return GameHelper:GetLanguageSwitchDes(mConfig.name)
		}
	}
	return "";
}

//int32 ThemeDataHandler::GetBgUnLock_ForStageRemainWinCount(csv_stagereward::RowData mTargetConfig)
//{
//	ensure(mTargetConfig.itemunlock == 2);
//	auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_stagereward>()->GetTable();
//	for (auto v : mTable)
//	{
//		auto mConfig = v;
//		if (mConfig.rewardid == mTargetConfig.id)
//		{
//			return mConfig.wincount - DataCenter::GetSingleton()->data->mStageReward_WinCount;
//		}
//	}
//	return 0;
//}

int32 ThemeDataHandler::GetThemePaiId_ForBgId(int32 Internalid)
{
	int32 tableid = this->GetThemeElements_Id_byInternalidAndType(Internalid, 1);
	auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_table2element>()->GetTable();
	for (auto& v : *mTable)
	{
		auto& mConfig = v;
		if (mConfig.tableid == tableid)
		{
			return this->GetThemeElements_internalid_byId(mConfig.paiid);
		}
	}
	
	ensureMsgf(false, TEXT("%d %d"), Internalid, tableid);
	return -1;
}

FDT_theme* ThemeDataHandler::GetUsedThemeConfig()
{
	auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_theme>()->GetTable();
	for (auto& v : *mTable)
	{
		auto& mConfig = v;
		if (
			mConfig.table == this->GetThemeElements_Id_byInternalidAndType(this->GetData()->themeBgId, 1) &&
			mConfig.front == this->GetThemeElements_Id_byInternalidAndType(this->GetData()->themeZhengId, 2) &&
			mConfig.back == this->GetThemeElements_Id_byInternalidAndType(this->GetData()->themeBackId, 3)
			)
		{
			return &mConfig;
		}
	}
	return nullptr;
}

FDT_theme* ThemeDataHandler::GetThemeConfigById(int32 nThemeId)
{
	auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_theme>()->GetTable();
	for (auto& v : *mTable)
	{
		auto& mConfig = v;
		if (mConfig.id == nThemeId)
		{
			return &mConfig;
		}
	}
	return nullptr;
}

//
int32 ThemeDataHandler::GetThemeElements_internalid_byId(int32 id)
{
	auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_themeitem>()->GetTable();
	for (auto& v : *mTable)
	{
		auto& mConfig = v;
		if (mConfig.id == id)
		{
			return mConfig.internalid;
		}
	}

	ensureMsgf(false, TEXT("%d"), id);
	return -1;
}

//--type 1:Bg 2 : front 3 : back 4 : 主题元素
int32 ThemeDataHandler::GetThemeElements_Id_byInternalidAndType(int32 Internalid, int32 type)
{
	auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_themeitem>()->GetTable();
	for (auto& v : *mTable)
	{
		auto& mConfig = v;
		if (mConfig.internalid == Internalid && mConfig.type == type)
		{
			return mConfig.id;
		}
	}
	
	ensureMsgf(false, TEXT("%d"), Internalid);
	return -1;
}

FString ThemeDataHandler::GetThemeThumbPath(int32 nId, int32 nType)
{
	if (nType == 0)
	{
		return FString::Printf(TEXT("thumb_theme_%d"), nId);
	}
	else if (nType == 1)
	{
		return FString::Printf(TEXT("thumb_bg_%d"), nId);
	}
	else if (nType == 2)
	{
		return FString::Printf(TEXT("thumb_card_%d"), nId);
	}
	else if (nType == 3)
	{
		return FString::Printf(TEXT("thumb_cardback_%d"), nId);
	}
	else
	{
		ensureMsgf(false, TEXT("%d"), nType);
	}

	return FString();
}

FString ThemeDataHandler::GetGamePoolBg_AtlasSpriteName(int32 nPaiId)
{
	return FString::Printf(TEXT("zhuti_%d_pai1"), nPaiId);
}

FString ThemeDataHandler::GetGameResultBg_AtlasSpriteName(int32 nPaiId)
{
	return FString::Printf(TEXT("zhuti_%d_pai3"), nPaiId);
}

FString ThemeDataHandler::GetGameLongBg_AtlasSpriteName(int32 nPaiId)
{
	return FString::Printf(TEXT("zhuti_%d_pai2"), nPaiId);
}
