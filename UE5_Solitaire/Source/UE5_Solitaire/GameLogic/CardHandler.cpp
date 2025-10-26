#include "CardHandler.h"

void CardHandler::Init()
{
	  
}

//local SpecialCardList = require "Lua.MainLogic.data.SpecialCardList"
TArray<int> CardHandler::GetInitCards_ForNormalMode()
{
    int nDifficultLayer = DataCenter::GetSingleton()->data.nDifficultLayer;
    int nGameLevel = DataCenter::GetSingleton()->data.nGameLevel;
    nGameLevel = FMath::Max(nGameLevel, 1);

    //if (nGameLevel <= #SpecialCardList)
    //{
    //    return SpecialCardList[nGameLevel];
    //}
    //else if (nGameLevel <= 50)
    //{
    //    local tableSimpleLevelData = RobotTestGenJsonFile.tableSimpleLevelData
    //    if (tableSimpleLevelData and #tableSimpleLevelData > 0)
    //    {
    //        return tableSimpleLevelData[math.random(1, #tableSimpleLevelData)]
    //    }
    //}

    ensureMsgf(false, TEXT("New RobotTestGetCardList"));
    return this->GetInitCards_ExcelRandom(nDifficultLayer, nGameLevel);
}

TArray<int> CardHandler::GetInitCards_ForRankMode()
{
    int64 nSeed = FDateTime::UtcNow().ToUnixTimestamp() / (3600 * 24);
    KKRandom mRandom = KKRandom(nSeed);

    TArray<int> tableCards = {};
    for (int i = 1; i < 13; i++)
    {
        for (int j = 1; k <= 4; j++) // 1:♣️，2:♥️ 3 : ♦️ 4：♠️
        {
            int nPokerId = this->GetPokerId(i, j);
            tableCards.Add(nPokerId);
        }
    }

    TArray<int> mSendCardList = {};
    while (tableCards.Num() > 0)
    {
        int nRemoveIndex = mRandom.RandomInt(1, tableCards.Num());
        int nPokerId = tableCards[nRemoveIndex];
        tableCards.RemoveAt(nRemoveIndex);
        mSendCardList.Add(nPokerId);
    }

    return mSendCardList;
}

TArray<int> CardHandler::GetInitCards_ForChallengeMode()
{
    int nGameLevel = DataCenter::GetSingleton()->data.nGameLevel;
    int nDifficultLayer = KKRandomTool::GetSingleton()->RandomInt(2, 10);
    return GetInitCards_ExcelRandom(nDifficultLayer, nGameLevel);
}

TArray<int> CardHandler::GetInitCards_ExcelRandom(int nDifficultLayer, int nGameLevel)
{
    int nDifficultLayer = FMath::Clamp(nDifficultLayer, 1, 10);
    nGameLevel = FMath::Max(nGameLevel, 1);

    TArray<int> tableIndex = {};
    for (int k = 0; k < CSV_jianhuan_vita::mTable.Num(); k++)
    {
        CSV_jianhuan_vita::RowData v = CSV_jianhuan_vita::mTable[k];
        if (v.layer == nDifficultLayer)
        {
            tableIndex.Add(k);
        }
    }

    if (tableIndex.Num() > 0)
    {
        int nRandomIndex = tableIndex[FMath::RandRange(0, tableIndex.Num())];
        TArray<int> tablePokerId = GetExcelTablePokerId(ExcelConfigHandler.Config_jianhuan_vita[nRandomIndex]);
        if (tablePokerId)
        {
            return tablePokerId
        }
    }

    return GetInitCards_Random();
}

TArray<int> CardHandler::GetInitCards_Random()
{
    TArray<int> tableCards = {};
    for (int i = 1; i < 13; i++)
    {
        for (int j = 1; j < 4; j++)  // 1 : ♣️，2: ♥️ 3 : ♦️ 4：♠️
        {
            int nPokerId = this->GetPokerId(i, j);
            tableCards.Add(nPokerId);
        }
    }

    TArray<int> mSendCardList = {};
    while (tableCards.Num() > 0)
    {
        int nRemoveIndex = FMath::RandRange(0, tableCards.Num() - 1);
        int nPokerId = tableCards[nRemoveIndex];
        tableCards.RemoveAt(nRemoveIndex);
        mSendCardList.Add(nPokerId);
    }
    return mSendCardList;
}

int CardHandler::GetPokerId(int nDigitId, int nColorType)
{
    return nDigitId * 10 + nColorType;
}

int CardHandler::GetDigital(int nPokerId)
{
    return nPokerId; // 10
}

int CardHandler::GetSubDigital(int nPokerId)
{
    return nPokerId % 10;
}

std::tuple<int, int> CardHandler::GetTwoDigital(int nPokerId)
{
    return { this->GetDigital(nPokerId), this->GetSubDigital(nPokerId) };
}

int CardHandler::GetColor(int nPokerId)
{
    int nSubDigital = this->GetSubDigital(nPokerId);
    if (nSubDigital == 2 or nSubDigital == 3)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

//----------------------------------------------------------Excel 数据解析----------------------------------------------------------------------------------
TArray<int> CardHandler::ConfusingDic = {
    27,53,95,52,66,62,0,33,68,5,20,85,92,
    35,40,22,43,31,23,57,80,55,9,98,67,58,
    97,65,99,3,75,34,90,16,70,60,25,79,86,
    69,29,76,28,12,50,39,42,59,96,63,26,17
};
CardHandler::ConfusingDic = LuaHelper.GetKeyValueSwitchTable(ConfusingDic1);

int CardHandler::GetExcelToLocalPokerId(int nExcelNum)
{
    int cardNum = ConfusingDic[nExcelNum] + 1;
    ensureMsgf(cardNum, TEXT("Error: %d"), nExcelNum);

    SolitaireColorType nColorType;
    if (cardNum >= 1 and cardNum <= 13)
    {
        nColorType = SolitaireColorType::Club;
    }
    else if (cardNum >= 14 and cardNum <= 26)
    {
        nColorType = SolitaireColorType::Heart;
    }
    else if (cardNum >= 27 and cardNum <= 39)
    {
        nColorType = SolitaireColorType::Diamond;
    }
    else if (cardNum >= 40 and cardNum <= 52)
    {
        nColorType = SolitaireColorType::Spade;
    }
    else
    {
        ensureMsgf(false, TEXT("Error"));
    }

    int nDigial = (cardNum - 1) % 13 + 1;
    int nPokerId = nDigial * 10 + nColorType;
    return nPokerId;
}

std::tuple<bool, TArray<int>> CardHandler::GetExcelTablePokerId_ForHalfWay(CSV_jianhuan_vita::RowData configItem)
{
    TArray<FString> tablePokerStr;
    configItem.jianhuanstr.ParseIntoArray(tablePokerStr, TEXT(","), false);
    ensureMsgf(tablePokerStr.Num() == 52, TEXT("Error"));

    TArray<int> tablePokerId = {};
    for (int i = 0; i < tablePokerStr.Num(); i++)
    {
        int number = FCString::Atoi(*tablePokerStr[i]);
        if (number > 0)
        {
            tablePokerId.Add(number);
        }
    }

    for (int i = 1; i < tablePokerId.Num(); i++)
    {
        int number = tablePokerId[i];
        int nPokerId = this->GetExcelToLocalPokerId(number);
        tablePokerId[i] = nPokerId;
    }

    if (this->CheckCardListError(tablePokerId) == false)
    {
        UE_LOG(LogTemp, Error, TEXT("CheckCardListError: %d"), configItem.sid);
        return { false, tablePokerId };
    }

    this->reverseTable(tablePokerId, 0, tablePokerId.Num() - 1);
    return { true, tablePokerId };
}

std::tuple<bool, TArray<int>> CardHandler::GetExcelTablePokerId(CSV_jianhuan_vita::RowData configItem)
{
    TArray<FString> tablePokerStr;
    configItem.jianhuanstr.ParseIntoArray(tablePokerStr, TEXT(","), false);
    ensureMsgf(tablePokerStr.Num() == 52, TEXT("Error"));

    TArray<int> tablePokerId = {};
    for (int i = 0; i < tablePokerStr.Num(); i++)
    {
        int number = FCString::Atoi(*tablePokerStr[i]);
        if (number > 0)
        {
            tablePokerId.Add(number);
        }
    }

    for (int i = 0; i < tablePokerId.Num(); i++)
    {
        int number = tablePokerId[i];
        int nPokerId = GetExcelToLocalPokerId(number);
        tablePokerId[i] = nPokerId;
    }

    if (this->CheckCardListError(tablePokerId) == false)
    {
        UE_LOG(LogTemp, Error, TEXT("CheckCardListError: %d"), configItem.sid);
        return {false, tablePokerId};
    }

    this->reverseTable(tablePokerId, tablePokerId.Num() - 24, tablePokerId.Num() - 1);
    this->reverseTable(tablePokerId, 0, tablePokerId.Num() - 1);
    ensureMsgf(tablePokerId.Num() == 52, TEXT("Error: %d %d"), tablePokerId.Num(), configItem.sid);
    return {true, tablePokerId };
}

void CardHandler::reverseTable(TArray<int>& mCardList, int nBeginIndex, int nEndIndex)
{
    while (nBeginIndex < nEndIndex)
    {
        int temp = mCardList[nBeginIndex];
        mCardList[nBeginIndex] = mCardList[nEndIndex];
        mCardList[nEndIndex] = temp;

        nBeginIndex = nBeginIndex + 1;
        nEndIndex = nEndIndex - 1;
    }
}

bool CardHandler::CheckCardListError(const TArray<int>& mCardList)
{
    if (mCardList.Num() != 52)
    {
        return false;
    }
            
    TArray<int> taleCardId = this->GetInitCards_Random();
    for (int i = 0; i < 52; i++)
    {
        int nPokerId = taleCardId[i];
        if (mCardList.Contains(nPokerId))
        {
            return false;
        }
    }

    return true;
}

