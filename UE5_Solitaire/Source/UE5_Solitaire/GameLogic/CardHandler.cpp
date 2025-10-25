#include "CardHandler.h"

void CardHandler::Init()
{
	
}


local SpecialCardList = require "Lua.MainLogic.data.SpecialCardList"
function CardHandler : GetInitCards_ForNormalMode()
    local nDifficultLayer = ThemeSolitaire.DataCenter.data.nDifficultLayer
    local nGameLevel = ThemeSolitaire.DataCenter.data.nGameLevel

    nGameLevel = math.max(nGameLevel, 1)
    if nGameLevel <= #SpecialCardList then
    return SpecialCardList[nGameLevel]
    elseif nGameLevel <= 50 then
    local tableSimpleLevelData = ThemeSolitaire.RobotTestGenJsonFile.tableSimpleLevelData
    if tableSimpleLevelData and #tableSimpleLevelData > 0 then
    return tableSimpleLevelData[math.random(1, #tableSimpleLevelData)]
    end
end

Debug.Assert(false, "New RobotTestGetCardList")
return self:GetInitCards_ExcelRandom(nDifficultLayer, nGameLevel)
end

function CardHandler : GetInitCards_ForRankMode()
local nSeed = TimeHelper.GetTimeStamp() // (3600 * 24)
local mRandom = CS.RandomUtility2(nSeed)

local tableCards = {}
for i = 1, 13 do
for j = 1, 4 do -- 1:♣️，2:♥️ 3 : ♦️ 4：♠️
local nPokerId = self : GetPokerId(i, j)
table.insert(tableCards, nPokerId)
end
end

local mSendCardList = {}
while #tableCards > 0 do
local nPokerId = table.remove(tableCards, mRandom:Random(1, #tableCards))
table.insert(mSendCardList, nPokerId)
end
return mSendCardList
end

function CardHandler : GetInitCards_ForChallengeMode()
local nGameLevel = ThemeSolitaire.DataCenter.data.nGameLevel
local nDifficultLayer = math.random(2, 10)
return self : GetInitCards_ExcelRandom(nDifficultLayer, nGameLevel)
end

function CardHandler : GetInitCards_ForTripMode()
local nTripTaskType = ThemeSolitaire.TripHandler.data.nTaskType
if nTripTaskType == 10 then
local ConfigList = ThemeSolitaire.ExcelConfigHandler.Config_jianhuan_halfway_source
local nIndex = math.random(1, #ConfigList)
local tablePokerId = CardHandler : GetExcelTablePokerId_ForHalfWay(ConfigList[nIndex])
return tablePokerId
else
local ConfigList = ThemeSolitaire.ExcelConfigHandler.Config_jianhuan_journeyGame_source
local nIndex = math.random(1, #ConfigList)
local tablePokerId = CardHandler:GetExcelTablePokerId(ConfigList[nIndex])
return tablePokerId
end
end

function CardHandler : GetInitCards_ExcelRandom(nDifficultLayer, nGameLevel)
nDifficultLayer = LuaHelper.Clamp(nDifficultLayer, 1, 10)
nGameLevel = math.max(nGameLevel, 1)

local tableIndex = {}
for k, v in ipairs(ThemeSolitaire.ExcelConfigHandler.Config_jianhuan_vita) do
if v.layer == nDifficultLayer then
table.insert(tableIndex, k)
end
end

if #tableIndex > 0 then
local nRandomIndex = tableIndex[math.random(1, #tableIndex)]
local tablePokerId = CardHandler:GetExcelTablePokerId(ThemeSolitaire.ExcelConfigHandler.Config_jianhuan_vita[nRandomIndex])
if tablePokerId then
return tablePokerId
end
end

return self:GetInitCards_Random()
end

function CardHandler : GetInitCards_Random_ForEasy()
local nDifficultLayer = ThemeSolitaire.DataCenter.data.nDifficultLayer
local nGameLevel = ThemeSolitaire.DataCenter.data.nGameLevel

local mSendCardList = self : GetInitCards_Random()
local randomRate = {
    13, 12, 11, 10, 9,
    8, 7, 6, 5, 4,
    3, 2, 1
}

local nMaxRate = 14 - nDifficultLayer
nMaxRate = math.max(1, nMaxRate)
for i = 1, #randomRate do
if randomRate[i] > nMaxRate then
randomRate[i] = nMaxRate
end
end

local mSendCardList2 = {}
while #mSendCardList > 0 do
local nRandomDigital = LuaHelper.GetIndexByRate(randomRate)

local nRemoveIndex = -1
for k, v in ipairs(mSendCardList) do
if self:GetDigital(v) == nRandomDigital then
nRemoveIndex = k
break
end
end

if nRemoveIndex > 0 then
local nPokerId = table.remove(mSendCardList, nRemoveIndex)
table.insert(mSendCardList2, nPokerId)
else
randomRate[nRandomDigital] = 0
end
end
return mSendCardList2
end

function CardHandler : GetInitCards_Random()
local tableCards = {}
for i = 1, 13 do
for j = 1, 4 do -- 1 : ♣️，2 : ♥️ 3 : ♦️ 4：♠️
local nPokerId = self : GetPokerId(i, j)
table.insert(tableCards, nPokerId)
end
end

local mSendCardList = {}
while #tableCards > 0 do
local nPokerId = table.remove(tableCards, math.random(1, #tableCards))
table.insert(mSendCardList, nPokerId)
end
return mSendCardList
end

function CardHandler : GetPokerId(nDigitId, nColorType)
return nDigitId * 10 + nColorType
end

function CardHandler : GetDigital(nPokerId)
return nPokerId // 10
end

function CardHandler : GetSubDigital(nPokerId)
return nPokerId % 10
end

function CardHandler : GetTwoDigital(nPokerId)
return self : GetDigital(nPokerId), self : GetSubDigital(nPokerId)
end

function CardHandler : GetColor(nPokerId)
local nSubDigital = self : GetSubDigital(nPokerId)
if nSubDigital == 2 or nSubDigital == 3 then
return 1
else
return 2
end
end

----------------------------------------------------------Excel 数据解析----------------------------------------------------------------------------------
local ConfusingDic = { 27,53,95,52,66,62,0,33,68,5,20,85,92,35,40,22,43,31,23,57,80,55,9,98,67,58,97,65,99,3,75,34,90,16,70,60,25,79,86,69,29,76,28,12,50,39,42,59,96,63,26,17 }
ConfusingDic = LuaHelper.GetKeyValueSwitchTable(ConfusingDic)
function CardHandler : GetExcelToLocalPokerId(nExcelNum)
--//解析cardnum
local cardNum = ConfusingDic[nExcelNum]
Debug.Assert(cardNum, "Error: ", nExcelNum)

local nColorType = -1;
if cardNum >= 1 and cardNum <= 13 then
nColorType = self.ColorType.Club
elseif cardNum >= 14 and cardNum <= 26 then
nColorType = self.ColorType.Heart
elseif cardNum >= 27 and cardNum <= 39 then
nColorType = self.ColorType.Diamond
elseif cardNum >= 40 and cardNum <= 52 then
nColorType = self.ColorType.Spade
else
Debug.Assert(false, "Error")
end

local nDigial = (cardNum - 1) % 13 + 1
local nPokerId = nDigial * 10 + nColorType
return nPokerId
end

function CardHandler : GetExcelTablePokerId_ForHalfWay(configItem)
local tablePokerStr = LuaHelper.StringSplit(configItem.jianhuanstr, ",")
Debug.Assert(#tablePokerStr == 52, "Error")

local tablePokerId = {}
for i = 1, #tablePokerStr do
local number = tonumber(tablePokerStr[i])
if number then
table.insert(tablePokerId, number)
end
end

for i = 1, #tablePokerId do
local number = tablePokerId[i]
local nPokerId = self:GetExcelToLocalPokerId(number)
tablePokerId[i] = nPokerId
end

if self : CheckCardListError(tablePokerId) == false then
Debug.LogError("CheckCardListError: ", configItem.sid)
return nil
end

self : reverseTable(tablePokerId, 1, #tablePokerId)
return tablePokerId
end

function CardHandler : GetExcelTablePokerId(configItem)
local tablePokerStr = LuaHelper.StringSplit(configItem.jianhuanstr, ",")
Debug.Assert(#tablePokerStr == 52, "Error")

local tablePokerId = {}
for i = 1, #tablePokerStr do
local number = tonumber(tablePokerStr[i])
if number then
table.insert(tablePokerId, number)
end
end

for i = 1, #tablePokerId do
local number = tablePokerId[i]
local nPokerId = self:GetExcelToLocalPokerId(number)
tablePokerId[i] = nPokerId
end

if self : CheckCardListError(tablePokerId) == false then
Debug.LogError("CheckCardListError: ", configItem.sid)
return nil
end

self : reverseTable(tablePokerId, #tablePokerId - 24 + 1, #tablePokerId)
self : reverseTable(tablePokerId, 1, #tablePokerId)
Debug.Assert(#tablePokerId == 52, "Error: ", #tablePokerId, configItem.sid)
return tablePokerId
end

function CardHandler : reverseTable(t, nBeginIndex, nEndIndex)
while nBeginIndex < nEndIndex do
    local temp = t[nBeginIndex]
    t[nBeginIndex] = t[nEndIndex]
    t[nEndIndex] = temp

    nBeginIndex = nBeginIndex + 1
    nEndIndex = nEndIndex - 1
    end
    end

    function CardHandler : CheckCardListError(mCardList)
    if mCardList == nil or #mCardList ~= 52 then return false end

        local taleCardId = self:GetInitCards_Random()
        for i = 1, 52 do
            local nPokerId = taleCardId[i]
            if not LuaHelper.tableContainsElement(mCardList, nPokerId) then
                return false
                end
                end
                return true
                end

