#include "StatisticDataHandler.h"

void StatisticDataHandler::Init()
{
    
}

FStatisticData StatisticDataHandler::GetDefaultData()
{
    FStatisticData tt = {};
    tt.nGameCount = 0;
    tt.nGameWinCount = 0;
    tt.nSumScore = 0;
    tt.nAverageScore = 0;
    tt.nMaxScore = 0;
    tt.nSumMoveCount = 0;
    tt.nAverageMoveCount = 0;
    tt.nMinMoveCount = 0;
    tt.nSumTime = 0;
    tt.nAverageTime = 0;
    tt.nMinTime = 0;
    tt.nNowWinningStreakCount = 0;
    tt.nMaxWinningStreakCount = 0;
    return tt;
}

void StatisticDataHandler::InitData()
{
    DataCenter::GetSingleton()->GetData()->StatisticData = {};
}

FStatisticData* StatisticDataHandler::GetData()
{
    return &DataCenter::GetSingleton()->GetData()->StatisticData;
}

void StatisticDataHandler::SetDbDataWithMeta()
{
    
}

void StatisticDataHandler::ResetData()
{
    this->InitData();
}

void StatisticDataHandler::onGameStatistic(bool bWin, int32 nMove, int32 nTime, int32 nScore)
{
    GetData()->nGameCount = GetData()->nGameCount + 1;
    if (bWin)
    {
        GetData()->nGameWinCount = GetData()->nGameWinCount + 1;

        GetData()->nSumScore = GetData()->nSumScore + nScore;
        GetData()->nAverageScore = GetData()->nSumScore / GetData()->nGameWinCount;
        if (GetData()->nMaxScore < nScore)
        {
            GetData()->nMaxScore = nScore;
        }

        GetData()->nSumMoveCount = GetData()->nSumMoveCount + nMove;
        GetData()->nAverageMoveCount = GetData()->nSumMoveCount / GetData()->nGameWinCount;
        if (GetData()->nMinMoveCount == 0 || GetData()->nMinMoveCount > nMove)
        {
            GetData()->nMinMoveCount = nMove;
        }

        GetData()->nSumTime = GetData()->nSumTime + nTime;
        GetData()->nAverageTime = GetData()->nSumTime / GetData()->nGameWinCount;
        if (GetData()->nMinTime == 0 || GetData()->nMinTime > nTime)
        {
            GetData()->nMinTime = nTime;
        }

        GetData()->nNowWinningStreakCount = GetData()->nNowWinningStreakCount + 1;
        if (GetData()->nMaxWinningStreakCount < GetData()->nNowWinningStreakCount)
        {
            GetData()->nMaxWinningStreakCount = GetData()->nNowWinningStreakCount;
        }
    }
    else
    {
        GetData()->nNowWinningStreakCount = 0;
    }
}