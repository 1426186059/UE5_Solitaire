#include "StatisticDataHandler.h"

void StatisticDataHandler::Init()
{
	UStatisticData* mmmData = DataCenter::GetSingleton()->data->StatisticData;
	if (mmmData == nullptr)
	{
		this->data = NewObject<UStatisticData>(DataCenter::GetSingleton()->data);
		DataCenter::GetSingleton()->data->StatisticData = this->data;
	}
	else
	{
		this->data = mmmData;
	}
}

UStatisticData* StatisticDataHandler::GetDefaultData()
{
    UStatisticData* tt = NewObject<UStatisticData>();
    tt->nGameCount = 0;
    tt->nGameWinCount = 0;
    tt->nSumScore = 0;
    tt->nAverageScore = 0;
    tt->nMaxScore = 0;
    tt->nSumMoveCount = 0;
    tt->nAverageMoveCount = 0;
    tt->nMinMoveCount = 0;
    tt->nSumTime = 0;
    tt->nAverageTime = 0;
    tt->nMinTime = 0;
    tt->nNowWinningStreakCount = 0;
    tt->nMaxWinningStreakCount = 0;
    return tt;
}

void StatisticDataHandler::SetDbDataWithMeta()
{
    DataCenter::GetSingleton()->data->StatisticData = this->data;
}

void StatisticDataHandler::ResetData()
{
    this->data = this->GetDefaultData();
}

void StatisticDataHandler::onGameStatistic(bool bWin, int32 nMove, int32 nTime, int32 nScore)
{
    data->nGameCount = data->nGameCount + 1;
    if (bWin)
    {
        data->nGameWinCount = data->nGameWinCount + 1;

        data->nSumScore = data->nSumScore + nScore;
        data->nAverageScore = data->nSumScore / data->nGameWinCount;
        if (data->nMaxScore < nScore)
        {
            data->nMaxScore = nScore;
        }

        data->nSumMoveCount = data->nSumMoveCount + nMove;
        data->nAverageMoveCount = data->nSumMoveCount / data->nGameWinCount;
        if (data->nMinMoveCount == 0 || data->nMinMoveCount > nMove)
        {
            data->nMinMoveCount = nMove;
        }

        data->nSumTime = data->nSumTime + nTime;
        data->nAverageTime = data->nSumTime / data->nGameWinCount;
        if (data->nMinTime == 0 || data->nMinTime > nTime)
        {
            data->nMinTime = nTime;
        }

        data->nNowWinningStreakCount = data->nNowWinningStreakCount + 1;
        if (data->nMaxWinningStreakCount < data->nNowWinningStreakCount)
        {
            data->nMaxWinningStreakCount = data->nNowWinningStreakCount;
        }
    }
    else
    {
        data->nNowWinningStreakCount = 0;
    }
}