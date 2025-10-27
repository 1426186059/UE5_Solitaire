#include "ThemeDataHandler.h"

void ThemeDataHandler::Init()
{
	UThemeData* mmmData = DataCenter::GetSingleton()->data->ThemeData;
	if (mmmData == nullptr)
	{
		this->data = NewObject<UThemeData>();
		DataCenter::GetSingleton()->data->ThemeData = this->data;
	}
	else
	{
		this->data = mmmData;
	}
}

//void ThemeDataHandler::InitDefaultTheme()
//{
//	if (this->data.themeBgId <= 0)
//	{
//		auto mThemeConfig = CSVConfigMgr::GetSingleton()->GetCSV<CSV_jianhuan_vita>()->GetTable();
//		this->UnLockNewTheme(mThemeConfig);
//		this->UseNewTheme(mThemeConfig);
//	}
//}
//
//function ThemeDataHandler : InitDefaultThemeElement()
//	for k, v  in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_theme) do
//	local mConfig = v;
//	if self:hasThemeid(mConfig.id) then
//	self : addbgid(self : GetThemeElements_internalid_byId(mConfig.table))
//	self : addfrontid(self : GetThemeElements_internalid_byId(mConfig.front))
//	self : addbackid(self : GetThemeElements_internalid_byId(mConfig.back))
//	end
//end
//
//self : RefreshThemePai()
//end
//
//function ThemeDataHandler : UnLockNewTheme(mThemeConfig)
//self : addThemeid(mThemeConfig.id);
//self:InitDefaultThemeElement()
//end
//
//function ThemeDataHandler : UseNewTheme(mThemeConfig)
//self.data.themeBgId = self : GetThemeElements_internalid_byId(mThemeConfig.table)
//self.data.themeBackId = self : GetThemeElements_internalid_byId(mThemeConfig.back)
//self.data.themeZhengId = self : GetThemeElements_internalid_byId(mThemeConfig.front)
//self : RefreshThemePai()
//GameEventHandler : Brocast(ThemeSolitaire.EventName.UpdatePokerAtlas)
//end
//
//function ThemeDataHandler : UseNewThemeBg(Id)
//self.data.themeBgId = Id;
//self:RefreshThemePai();
//
//
//GameEventHandler:Brocast(ThemeSolitaire.EventName.UpdatePokerAtlas)
//end
//
//function ThemeDataHandler : UseNewThemeFront(Id)
//self.data.themeZhengId = Id;
//
//
//GameEventHandler:Brocast(ThemeSolitaire.EventName.UpdatePokerAtlas)
//end
//
//function ThemeDataHandler : UseNewThemeBack(Id)
//self.data.themeBackId = Id;
//
//GameEventHandler:Brocast(ThemeSolitaire.EventName.UpdatePokerAtlas)
//end
//
//function ThemeDataHandler : RefreshThemePai()
//local themeBgId = self.data.themeBgId
//if themeBgId > 0 then
//self.data.themePaiId = self:GetThemePaiId_ForBgId(themeBgId)
//end
//end
//
//-- 是否主题中的元素，比如桌面Bg，牌面，牌背，跟随主题解锁
//function ThemeDataHandler : orThemeUnLockToUnLockMe(mTargetConfig)
//Debug.Assert(mTargetConfig.itemunlock == 4)
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_theme) do
//local mConfig = v
//if mConfig.table == mTargetConfig.id then
//return self : hasThemeid(mConfig.id);
//elseif mConfig.back == mTargetConfig.id then
//return self:hasThemeid(mConfig.id);
//elseif mConfig.front == mTargetConfig.id then
//return self:hasThemeid(mConfig.id);
//end
//end
//return false;
//end
//
//function ThemeDataHandler : GetBgThemeName(mTargetConfig)
//Debug.Assert(mTargetConfig.itemunlock == 4)
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_theme) do
//local mConfig = v
//if mConfig.table == mTargetConfig.id then
//return GameHelper : GetLanguageSwitchDes(mConfig.name)
//end
//end
//return "";
//end
//
//function ThemeDataHandler : GetBgUnLock_ForStageRemainWinCount(mTargetConfig)
//Debug.Assert(mTargetConfig.itemunlock == 2);
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_stagereward) do
//local mConfig = v
//if mConfig.rewardid == mTargetConfig.id then
//return mConfig.wincount - ThemeSolitaire.DataCenter.data.mStageReward_WinCount;
//end
//end
//return 0
//end
//
//function ThemeDataHandler : GetThemePaiId_ForBgId(Internalid)
//local tableid = self : GetThemeElements_Id_byInternalidAndType(Internalid, 1)
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_table2element) do
//local mConfig = v
//if mConfig.tableid == tableid then
//return self : GetThemeElements_internalid_byId(mConfig.paiid);
//end
//end
//
//Debug.Assert(false, Internalid, tableid);
//return -1
//end
//
//function ThemeDataHandler : GetUsedThemeConfig()
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_theme) do
//local mConfig = v
//if mConfig.table == self : GetThemeElements_Id_byInternalidAndType(self.data.themeBgId, 1) and
//mConfig.front == self : GetThemeElements_Id_byInternalidAndType(self.data.themeZhengId, 2) and
//mConfig.back == self : GetThemeElements_Id_byInternalidAndType(self.data.themeBackId, 3) then
//return mConfig;
//end
//end
//return nil
//end
//
//function ThemeDataHandler : GetThemeConfigById(nThemeId)
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_theme) do
//local mConfig = v
//if mConfig.id == nThemeId then
//return mConfig
//end
//end
//return nil
//end
//
//function ThemeDataHandler : GetThemeElements_internalid_byId(id)
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_themeitem) do
//local mConfig = v
//if mConfig.id == id then
//return mConfig.internalid;
//end
//end
//Debug.Assert(false, id)
//return -1;
//end
//
//--type 1:Bg 2 : front 3 : back 4 : 主题元素
//function ThemeDataHandler : GetThemeElements_Id_byInternalidAndType(Internalid, type)
//for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_themeitem) do
//local mConfig = v
//if mConfig.internalid == Internalid and mConfig.type == type then
//return mConfig.id
//end
//end
//Debug.Assert(false, Internalid);
//return -1
//end
//
//function ThemeDataHandler : GetThemeThumbPath(nId, nType)
//if nType == 0 then
//return "thumb_theme_"..nId
//elseif nType == 1 then
//return "thumb_bg_"..nId
//elseif nType == 2 then
//return "thumb_card_"..nId
//elseif nType == 3 then
//return "thumb_cardback_"..nId
//else
//Debug.Assert(false, nType)
//end
//end
//
//function ThemeDataHandler : GetGamePoolBg_AtlasSpriteName(nPaiId)
//return "zhuti_"..nPaiId.."_pai1"
//end
//
//function ThemeDataHandler : GetGameResultBg_AtlasSpriteName(nPaiId)
//return "zhuti_"..nPaiId.."_pai3"
//end
//
//function ThemeDataHandler : GetGameLongBg_AtlasSpriteName(nPaiId)
//return "zhuti_"..nPaiId.."_pai2"
//end
