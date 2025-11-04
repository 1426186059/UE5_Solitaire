#include "RecordStepDataHandler.h"

void RecordStepDataHandler::Init()
{
	UE_LOG(LogTemp, Log, TEXT("RecordStepDataHandler nTime: %d"), this->GetData()->nTime);
	UE_LOG(LogTemp, Log, TEXT("RecordStepDataHandler tableOpStepItem Count: %d"), this->GetData()->tableOpStepItem.Num());
	this->InitData();
}

void RecordStepDataHandler::InitData()
{
	DataCenter::GetSingleton()->data->tableOpRecord = this->GetDefaultData();
}

FRecordStepData* RecordStepDataHandler::GetData()
{
	return &DataCenter::GetSingleton()->data->tableOpRecord;
}

FRecordStepData RecordStepDataHandler::GetDefaultData()
{
	FRecordStepData tt = {};
	tt.nGameMode = 0;
	tt.nMoveCount = 0;
	tt.nTime = 0;
	tt.nScore = 0;
	tt.bWin = false;
	tt.mInitSendCardList = {};
	tt.tableOpStepItem = {};
	tt.nUndoCount = 0;
	return tt;
}

FRecordStepData_OpStepItem RecordStepDataHandler::GetOpStepItemDefaultData()
{
	FRecordStepData_OpStepItem tt = {};
	tt.fromPosTypeInfo = {};
	tt.toPosTypeInfo = {};
	tt.mSelectCardList = {};
	tt.nScore = 0;
	tt.nTureOverPokerId = 0;
	return tt;
}

void RecordStepDataHandler::SetDbDataWithMeta()
{
	//DataCenter::GetSingleton()->data->tableOpRecord = this->GetData();
}

void RecordStepDataHandler::InitStepRecordFromOther(const FRecordStepData& mmmData)
{
	//ThemeSolitaire.AllRecordDataHandler : SaveGameRecordToDirectory()
	DataCenter::GetSingleton()->data->tableOpRecord = mmmData;
}

void RecordStepDataHandler::InitStepRecord(int32 nGameMode, const TArray<int>& mInitSendCardList)
{
	//ThemeSolitaire.AllRecordDataHandler : SaveGameRecordToDirectory()
	this->InitData();
	this->GetData()->mInitSendCardList = mInitSendCardList;
	this->GetData()->nGameMode = nGameMode;
}

void RecordStepDataHandler::AddUndoCount(int nCount)
{
	this->GetData()->nUndoCount += nCount;
	this->GetData()->nUndoCount = FMath::Clamp(this->GetData()->nUndoCount, 0, this->GetData()->tableOpStepItem.Num());
	UE_LOG(LogTemp, Log, TEXT("nUndoCount: %d"), this->GetData()->nUndoCount);
}

void RecordStepDataHandler::ResetUndoCountTo0()
{
	while (this->GetData()->nUndoCount > 0)
	{
		this->GetData()->tableOpStepItem.Pop();
		this->GetData()->nUndoCount = this->GetData()->nUndoCount - 1;
	}
}

void RecordStepDataHandler::AddStepRecord(const FRecordStepData_OpStepItem& mOpStepItemData)
{
	this->ResetUndoCountTo0();
	this->GetData()->tableOpStepItem.Add(mOpStepItemData);
	this->PrintOp(mOpStepItemData);
}

std::tuple<int32, FRecordStepData_OpStepItem*> RecordStepDataHandler::GetNowStepRecord()
{
	int32 nStepIndex = this->GetData()->tableOpStepItem.Num() - this->GetData()->nUndoCount;
	return { nStepIndex, &this->GetData()->tableOpStepItem[nStepIndex] };
}

bool RecordStepDataHandler::orCanUndo()
{
	return this->GetData()->tableOpStepItem.Num() > 0 && this->GetData()->tableOpStepItem.Num() > this->GetData()->nUndoCount;
}

bool RecordStepDataHandler::orCanForward()
{
	return this->GetData()->nUndoCount > 0;
}

FString RecordStepDataHandler::GetPosTypeName(SolitairePokerPosType nPosType)
{
	FString strOriName;
	if (nPosType == SolitairePokerPosType::Draw3Pos)
	{
		strOriName = "Draw3Pos";
	}
	else if (nPosType == SolitairePokerPosType::A4Pos)
	{
		strOriName = "A4Pos";
	}
	else if (nPosType == SolitairePokerPosType::SendPokerPos)
	{
		strOriName = "SendPokerPos";
	}
	else if (nPosType == SolitairePokerPosType::Top7Pos)
	{
		strOriName = "Top7Pos";
	}
	else
	{
		ensureMsgf(false, TEXT("%d"), nPosType);
	}
	return strOriName;
}

void RecordStepDataHandler::PrintOp(const FRecordStepData_OpStepItem& mOpStepItemData)
{
	//if LuaGameConfig.isMobilePlatform then return end

	int nIndex = this->GetData()->tableOpStepItem.IndexOfByKey(mOpStepItemData);
	FString info = FString::Printf(TEXT("操作步骤：%d: %s(%d, %d) => %s(%d, %d) Score: %d"),
		nIndex,
		*this->GetPosTypeName((SolitairePokerPosType)mOpStepItemData.fromPosTypeInfo[0]),
		mOpStepItemData.fromPosTypeInfo[1],
		mOpStepItemData.fromPosTypeInfo[2],
		*this->GetPosTypeName((SolitairePokerPosType)mOpStepItemData.toPosTypeInfo[0]),
		mOpStepItemData.toPosTypeInfo[1],
		mOpStepItemData.toPosTypeInfo[2],
		mOpStepItemData.nScore
	);
	UE_LOG(LogTemp, Log, TEXT("%s"), *info);
}

void RecordStepDataHandler::PrintAllOp()
{
	//if LuaGameConfig.isMobilePlatform then return end
	for (const auto& v : this->GetData()->tableOpStepItem)
	{
		this->PrintOp(v);
	}
}

void RecordStepDataHandler::AddMoveCount()
{
	this->GetData()->nMoveCount = this->GetData()->nMoveCount + 1;
}

void RecordStepDataHandler::AddScore(int32 nCount)
{
	this->GetData()->nScore = this->GetData()->nScore + nCount;
}

void RecordStepDataHandler::AddTime(int32 nCount)
{
	this->GetData()->nTime = this->GetData()->nTime + nCount;
}

void RecordStepDataHandler::SetLoseWin(bool bWin)
{
	this->GetData()->bWin = bWin;
}

bool RecordStepDataHandler::orGameEnd()
{
	return this->GetData()->mInitSendCardList.Num() == 0 || this->GetData()->bWin == true;
}