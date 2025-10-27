#include "RecordStepDataHandler.h"

void RecordStepDataHandler::Init()
{
	URecordStepData* mmmData = DataCenter::GetSingleton()->data->tableOpRecord;
	if (mmmData == nullptr)
	{
		this->data = NewObject<URecordStepData>();
		DataCenter::GetSingleton()->data->tableOpRecord = this->data;
	}
	else
	{
		this->data = mmmData;
	}
}

URecordStepData* RecordStepDataHandler::GetDefaultData()
{
	auto tt = NewObject<URecordStepData>();
	tt->nGameMode = 0;
	tt->nMoveCount = 0;
	tt->nTime = 0;
	tt->nScore = 0;
	tt->bWin = false;
	tt->mInitSendCardList = {};
	tt->tableOpStepItem = {};
	tt->nUndoCount = 0;
	return tt;
}

URecordStepData_OpStepItem* RecordStepDataHandler::GetOpStepItemDefaultData()
{
	auto tt = NewObject<URecordStepData_OpStepItem>();
	tt->fromPosTypeInfo = {};
	tt->toPosTypeInfo = {};
	tt->mSelectCardList = {};
	tt->nScore = 0;
	tt->nTureOverPokerId = 0;
	return tt;
}


void RecordStepDataHandler::SetDbDataWithMeta()
{
	DataCenter::GetSingleton()->data->tableOpRecord = this->data;
}

void RecordStepDataHandler::InitStepRecordFromOther(URecordStepData* mmmData)
{
	//ThemeSolitaire.AllRecordDataHandler : SaveGameRecordToDirectory()
	this->data = mmmData;
}

void RecordStepDataHandler::InitStepRecord(int32 nGameMode, TArray<int> mInitSendCardList)
{
	//ThemeSolitaire.AllRecordDataHandler : SaveGameRecordToDirectory()
	this->data = this->GetDefaultData();
	this->data->mInitSendCardList = mInitSendCardList;
	this->data->nGameMode = nGameMode;
}

void RecordStepDataHandler::AddUndoCount(int nCount)
{
	this->data->nUndoCount = this->data->nUndoCount + nCount;
	this->data->nUndoCount = FMath::Clamp(this->data->nUndoCount, 0, this->data->tableOpStepItem.Num());
	UE_LOG(LogTemp, Log, TEXT("nUndoCount: %d"), this->data->nUndoCount);
}

void RecordStepDataHandler::ResetUndoCountTo0()
{
	while (this->data->nUndoCount > 0)
	{
		this->data->tableOpStepItem.Pop();
		this->data->nUndoCount = this->data->nUndoCount - 1;
	}
}

void RecordStepDataHandler::AddStepRecord(URecordStepData_OpStepItem* mOpStepItemData)
{
	this->ResetUndoCountTo0();
	this->data->tableOpStepItem.Add(mOpStepItemData);
	this->PrintOp(mOpStepItemData);
}

std::tuple<int32, URecordStepData_OpStepItem*> RecordStepDataHandler::GetNowStepRecord()
{
	int32 nStepIndex = this->data->tableOpStepItem.Num() - this->data->nUndoCount;
	return { nStepIndex, this->data->tableOpStepItem[nStepIndex] };
}

bool RecordStepDataHandler::orCanUndo()
{
	return this->data->tableOpStepItem.Num() > 0 && this->data->tableOpStepItem.Num() > this->data->nUndoCount;
}

bool RecordStepDataHandler::orCanForward()
{
	return this->data->nUndoCount > 0;
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

void RecordStepDataHandler::PrintOp(URecordStepData_OpStepItem* mOpStepItemData)
{
	//if LuaGameConfig.isMobilePlatform then return end

	int nIndex = this->data->tableOpStepItem.IndexOfByKey(mOpStepItemData);
	FString info = FString::Printf(TEXT("操作步骤：%d: %s(%d, %d) => %s(%d, %d) Score: %d"),
		nIndex,
		*this->GetPosTypeName((SolitairePokerPosType)mOpStepItemData->fromPosTypeInfo[0]),
		mOpStepItemData->fromPosTypeInfo[1],
		mOpStepItemData->fromPosTypeInfo[2],
		*this->GetPosTypeName((SolitairePokerPosType)mOpStepItemData->toPosTypeInfo[0]),
		mOpStepItemData->toPosTypeInfo[1],
		mOpStepItemData->toPosTypeInfo[2],
		mOpStepItemData->nScore
	);
	UE_LOG(LogTemp, Log, TEXT("%s"), *info);
}

void RecordStepDataHandler::PrintAllOp()
{
	//if LuaGameConfig.isMobilePlatform then return end
	for (auto v : this->data->tableOpStepItem)
	{
		this->PrintOp(v);
	}
}

void RecordStepDataHandler::AddMoveCount()
{
	this->data->nMoveCount = this->data->nMoveCount + 1;
}

void RecordStepDataHandler::AddScore(int32 nCount)
{
	this->data->nScore = this->data->nScore + nCount;
}

void RecordStepDataHandler::AddTime(int32 nCount)
{
	this->data->nTime = this->data->nTime + nCount;
}

void RecordStepDataHandler::SetLoseWin(bool bWin)
{
	this->data->bWin = bWin;
}

bool RecordStepDataHandler::orGameEnd()
{
	return this->data->mInitSendCardList.Num() == 0 || this->data->bWin == true;
}