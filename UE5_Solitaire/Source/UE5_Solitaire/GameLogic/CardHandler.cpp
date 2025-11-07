#include "CardHandler.h"

void CardHandler::Init()
{
    InitConfusingDic();
}

TArray<int> CardHandler::GetInitCards_ForNormalMode()
{
    int nDifficultLayer = DataCenter::GetSingleton()->data->nDifficultLayer;
    int nGameLevel = DataCenter::GetSingleton()->data->nGameLevel;

    nDifficultLayer = FMath::Clamp(nDifficultLayer, 1, 10);
    nGameLevel = FMath::Max(nGameLevel, 1);

    if (false)
    {
        if (nGameLevel <= 10)
        {
            return this->GetInitCards_Random_ForEasy();
        }
    }
    else
    {
        if (nGameLevel <= 5)
        {
            auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_jianhuan_newbie>()->GetTable();
            auto mConfigItem = CardHandler::GetVitaConfigItem2(mTable[nGameLevel].sid);
            auto [bTrue, tablePokerId] = this->GetExcelTablePokerId(mConfigItem);
            if (bTrue)
            {
                return tablePokerId;
            }
            UE_LOG(LogTemp, Error, TEXT("SimpleLevel Error: %s"), *mTable[nGameLevel].sid);
        }
    }

    return this->GetInitCards_ExcelRandom(nDifficultLayer, nGameLevel);
}

TArray<int> CardHandler::GetInitCards_ForRankMode()
{
    int64 nSeed = KKTimeTool::GetTimeStamp() / (3600 * 24);
    KKRandom mRandom = KKRandom(nSeed);

    TArray<int> tableCards = {};
    for (int i = 1; i < 13; i++)
    {
        for (int j = 1; j <= 4; j++)     // 1:♣️，2:♥️ 3 : ♦️ 4：♠️
        {
            int nPokerId = this->GetPokerId(i, j);
            tableCards.Add(nPokerId);
        }
    }

    TArray<int> mSendCardList = {};
    while (tableCards.Num() > 0)
    {
        int nPokerId = TArrayExtentions::RandomRemove(tableCards);
        mSendCardList.Add(nPokerId);
    }

    return mSendCardList;
}

TArray<int> CardHandler::GetInitCards_ForChallengeMode()
{
    int nGameLevel = DataCenter::GetSingleton()->data->nGameLevel;
    int nDifficultLayer = KKRandomTool::RandomInt(2, 10);
    return GetInitCards_ExcelRandom(nDifficultLayer, nGameLevel);
}

TArray<int> CardHandler::GetInitCards_Random_ForEasy()
{
    int nDifficultLayer = DataCenter::GetSingleton()->data->nDifficultLayer;
    int nGameLevel = DataCenter::GetSingleton()->data->nGameLevel;

    TArray<int> mSendCardList = this->GetInitCards_Random();
    TArray<int> randomRate = {
        13, 12, 11, 10, 9,
        8, 7, 6, 5, 4,
        3, 2, 1
    };

    int nMaxRate = 10 - nDifficultLayer;
    nMaxRate = FMath::Max(1, nMaxRate);
    for (int i = 0; i < randomRate.Num(); i++)
    {
        if (randomRate[i] > nMaxRate)
        {
            randomRate[i] = nMaxRate;
        }
    }

    TArray<int> mSendCardList2 = {};
    while (mSendCardList.Num() > 0)
    {
        int nRandomIndex = KKRandomTool::GetIndexByRate(randomRate);
        int nRandomDigital = nRandomIndex + 1;

        int nRemoveIndex = -1;
        for (int i = 0; i < mSendCardList.Num(); i++)
        {
            int v = mSendCardList[i];
            if (this->GetDigital(v) == nRandomDigital)
            {
                nRemoveIndex = i;
                break;
            }
        }

        if (nRemoveIndex >= 0)
        {
            int nPokerId = TArrayExtentions::Remove(mSendCardList, nRemoveIndex);
            mSendCardList2.Add(nPokerId);
        }
        else
        {
            randomRate[nRandomIndex] = 0;
        }
    }

    if (this->CheckCardListError(mSendCardList2))
    {
        UE_LOG(LogTemp, Error, TEXT("GetInitCards_Random_ForEasy CheckCardListError"));
        return this->GetInitCards_Random();
    }

    return mSendCardList2;
}

csv_jianhuan_vita::RowData* CardHandler::GetVitaConfigItem(FString sid)
{
    auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_jianhuan_vita>()->GetTable();
    for (int k = 0; k < mTable.Num(); k++)
    {
        if (mTable[k].sid == sid)
        {
            return &mTable[k];
        }
    }
    return nullptr;
}

FDT_jianhuan_vita* CardHandler::GetVitaConfigItem2(FString sid)
{
    auto mTable = ADTMgr::GetSingleton()->Get<FDT_jianhuan_vita>();
    ensure(mTable);
    if (auto Row = mTable->FindRow<FDT_jianhuan_vita>(TEXT("sid"), sid))
    {
        return Row;
    }

    return nullptr;
}

TArray<int> CardHandler::GetInitCards_ExcelRandom(int nDifficultLayer, int nGameLevel)
{
    nDifficultLayer = FMath::Clamp(nDifficultLayer, 1, 10);
    nGameLevel = FMath::Max(nGameLevel, 1);

    auto mTable = CSVConfigMgr::GetSingleton()->GetCSV<csv_jianhuan_vita>()->GetTable();

    TArray<int> tableIndex = {};
    for (int k = 0; k < mTable.Num(); k++)
    {
        csv_jianhuan_vita::RowData v = mTable[k];
        if (v.layer == nDifficultLayer)
        {
            tableIndex.Add(k);
        }
    }

    if (tableIndex.Num() > 0)
    {
        int nRandomIndex = tableIndex[KKRandomTool::RandomArrayInt(tableIndex.Num())];
        auto [bTrue, tablePokerId] = this->GetExcelTablePokerId(&mTable[nRandomIndex]);
        if (bTrue)
        {
            return tablePokerId;
        }
    }

    return GetInitCards_Random();
}

TArray<int> CardHandler::GetInitCards_Random()
{
    TArray<int> tableCards = {};
    for (int i = 1; i <= 13; i++)
    {
        for (int j = 1; j <= 4; j++)  // 1 : ♣️，2: ♥️ 3 : ♦️ 4：♠️
        {
            int nPokerId = this->GetPokerId(i, j);
            tableCards.Add(nPokerId);
        }
    }

    TArray<int> mSendCardList = {};
    while (tableCards.Num() > 0)
    {
        int nPokerId = TArrayExtentions::RandomRemove(tableCards);
        mSendCardList.Add(nPokerId);
    }

    ensureMsgf(mSendCardList.Num() == 52, TEXT("%d"), mSendCardList.Num());
    return mSendCardList;
}

int CardHandler::GetPokerId(int nDigitId, int nColorType)
{
    return nDigitId * 10 + nColorType;
}

int CardHandler::GetDigital(int nPokerId)
{
    return nPokerId / 10;
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
    if (nSubDigital == 2 || nSubDigital == 3)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

//----------------------------------------------------------Excel 数据解析----------------------------------------------------------------------------------
void CardHandler::InitConfusingDic()
{
    TArray<int> ConfusingList = {
        27,53,95,52,66,62,0,33,68,5,20,85,92,
        35,40,22,43,31,23,57,80,55,9,98,67,58,
        97,65,99,3,75,34,90,16,70,60,25,79,86,
        69,29,76,28,12,50,39,42,59,96,63,26,17
    };

    ensure(ConfusingList.Num() == 52);
    this->ConfusingDic = {};
    for (int i = 0; i < ConfusingList.Num(); i++)
    {
        this->ConfusingDic.Add(ConfusingList[i], i + 1);
    }
}

int CardHandler::GetExcelToLocalPokerId(int nExcelNum)
{
    int cardNum = this->ConfusingDic[nExcelNum];
    ensureMsgf(cardNum >= 1 && cardNum <= 52, TEXT("Error: %d %d"), nExcelNum, cardNum);

    SolitaireColorType nColorType = SolitaireColorType::Club;
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
        ensureMsgf(false, TEXT("GetExcelToLocalPokerId Error: %d"), cardNum);
    }

    int nDigial = (cardNum - 1) % 13 + 1;
    int nPokerId = this->GetPokerId(nDigial, (int)nColorType);
    return nPokerId;
}

std::tuple<bool, TArray<int>> CardHandler::GetExcelTablePokerId_ForHalfWay(const csv_jianhuan_vita::RowData& configItem)
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
        int nPokerId = this->GetExcelToLocalPokerId(number);
        tablePokerId[i] = nPokerId;
    }

    if (this->CheckCardListError(tablePokerId))
    {
        UE_LOG(LogTemp, Error, TEXT("CheckCardListError: %s"), *configItem.sid);
        return { false, tablePokerId };
    }

    this->reverseTable(tablePokerId, 0, tablePokerId.Num() - 1);
    return { true, tablePokerId };
}

std::tuple<bool, TArray<int>> CardHandler::GetExcelTablePokerId(csv_jianhuan_vita::RowData* configItem)
{
    //UE_LOG(LogTemp, Error, TEXT("configItem->jianhuanstr:%d, %s, %s"), configItem->layer,  *configItem->sid, *configItem->jianhuanstr);
    TArray<FString> tablePokerStr;
    configItem->jianhuanstr.ParseIntoArray(tablePokerStr, TEXT(","), false);

    ensureMsgf(tablePokerStr.Num() == 52, TEXT("%d"), tablePokerStr.Num());
    TArrayExtentions::PrintTArray(tablePokerStr, "tablePokerStr");
    
    TArray<int> tablePokerId = {};
    for (int i = 0; i < tablePokerStr.Num(); i++)
    {
        int number = FCString::Atoi(*tablePokerStr[i]);
        tablePokerId.Add(number);
    }

    TArrayExtentions::PrintTArray(tablePokerId, "tablePokerId 000");
    ensureMsgf(tablePokerId.Num() == 52, TEXT("%d"), tablePokerId.Num());
    for (int i = 0; i < tablePokerId.Num(); i++)
    {
        int number = tablePokerId[i];
        int nPokerId = GetExcelToLocalPokerId(number);
        tablePokerId[i] = nPokerId;
    }

    TArrayExtentions::PrintTArray(tablePokerId, "tablePokerId 111");
    ensureMsgf(tablePokerId.Num() == 52, TEXT("%d"), tablePokerId.Num());
    if (this->CheckCardListError(tablePokerId))
    {
        UE_LOG(LogTemp, Error, TEXT("CheckCardListError: %s"), *configItem->sid);
        return { false, tablePokerId };
    }

    this->reverseTable(tablePokerId, tablePokerId.Num() - 24, tablePokerId.Num() - 1);
    this->reverseTable(tablePokerId, 0, tablePokerId.Num() - 1);
    ensureMsgf(tablePokerId.Num() == 52, TEXT("Error: %d %s"), tablePokerId.Num(), *configItem->sid);
    return { true, tablePokerId };
}

std::tuple<bool, TArray<int>> CardHandler::GetExcelTablePokerId(FDT_jianhuan_vita* configItem)
{
    UE_LOG(LogTemp, Error, TEXT("CheckCardListError: configItem->jianhuanstr: %s, %s"), *configItem->sid, *configItem->jianhuanstr);
    TArray<FString> tablePokerStr;
    configItem->jianhuanstr.ParseIntoArray(tablePokerStr, TEXT(","), false);

    ensureMsgf(tablePokerStr.Num() == 52, TEXT("%d"), tablePokerStr.Num());
    TArrayExtentions::PrintTArray(tablePokerStr, "tablePokerStr");

    TArray<int> tablePokerId = {};
    for (int i = 0; i < tablePokerStr.Num(); i++)
    {
        int number = FCString::Atoi(*tablePokerStr[i]);
        tablePokerId.Add(number);
    }

    TArrayExtentions::PrintTArray(tablePokerId, "tablePokerId 000");
    ensureMsgf(tablePokerId.Num() == 52, TEXT("%d"), tablePokerId.Num());
    for (int i = 0; i < tablePokerId.Num(); i++)
    {
        int number = tablePokerId[i];
        int nPokerId = GetExcelToLocalPokerId(number);
        tablePokerId[i] = nPokerId;
    }

    TArrayExtentions::PrintTArray(tablePokerId, "tablePokerId 111");
    ensureMsgf(tablePokerId.Num() == 52, TEXT("%d"), tablePokerId.Num());
    if (this->CheckCardListError(tablePokerId))
    {
        UE_LOG(LogTemp, Error, TEXT("CheckCardListError: %s"), *configItem->sid);
        return { false, tablePokerId };
    }

    this->reverseTable(tablePokerId, tablePokerId.Num() - 24, tablePokerId.Num() - 1);
    this->reverseTable(tablePokerId, 0, tablePokerId.Num() - 1);
    ensureMsgf(tablePokerId.Num() == 52, TEXT("Error: %d %s"), tablePokerId.Num(), *configItem->sid);
    return { true, tablePokerId };
}

void CardHandler::reverseTable(TArray<int>& mCardList, int nBeginIndex, int nEndIndex)
{
    while (nBeginIndex < nEndIndex)
    {
        int temp = mCardList[nBeginIndex];
        mCardList[nBeginIndex] = mCardList[nEndIndex];
        mCardList[nEndIndex] = temp;
        nBeginIndex++;
        nEndIndex--;
    }
}

bool CardHandler::CheckCardListError(const TArray<int>& mCardList)
{
    bool bHaveError = false;
    if (mCardList.Num() != 52)
    {
        UE_LOG(LogTemp, Error, TEXT("CheckCardListError: mCardList.Num() != 52:  %d"), mCardList.Num());
        bHaveError = true;
    }
    
    const TArray<int> taleCardId = this->GetInitCards_Random();
    for (int nPokerId : taleCardId)
    {
        if (!mCardList.Contains(nPokerId))
        {
            UE_LOG(LogTemp, Error, TEXT("CheckCardListError: mCardList.Contains(nPokerId):  %d"), nPokerId);
            bHaveError = true;
        }
    }

    return bHaveError;
}

