#include "AllRecordDataHandler.h"

const FString AllRecordDataHandler::GameRecordDirPath = "ThemeSolitaire/";
const FString AllRecordDataHandler::GameRecordFileIndexName = "ThemeSolitaire/index.txt";

void AllRecordDataHandler::Init()
{
	USaveGame* mFileIndex = UGameplayStatics::LoadGameFromSlot(AllRecordDataHandler::GameRecordFileIndexName, DataCenter::DATA_USER_INDEX);
	if (mFileIndex != nullptr)
	{
		this->data = Cast<UAllRecordData>(mFileIndex);
	}
	else
	{
		this->data = Cast<UAllRecordData>(UGameplayStatics::CreateSaveGameObject(UAllRecordData::StaticClass()));
	}

	UE_LOG(LogTemp, Log, TEXT("当前记录局数: %d"), this->data->tableFileNameItem.Num());
}

void AllRecordDataHandler::SaveDb()
{
	UGameplayStatics::SaveGameToSlot(this->data, AllRecordDataHandler::GameRecordFileIndexName, DataCenter::DATA_USER_INDEX);
}

void AllRecordDataHandler::SaveGameRecordToDirectory()
{
	auto mInitSendCardList = RecordStepDataHandler::GetSingleton()->data->mInitSendCardList;
	if (mInitSendCardList.Num() == 0)
	{
		return;
	}

	int64 nTimeStamp = KKTimeTool::GetTimeStamp();
	FString fileName = FString::Printf(TEXT("ThemeSolitaire/%d.txt"), nTimeStamp);
	auto mOpRecordFileData = Cast<UAllRecordData_OpRecord>(UGameplayStatics::CreateSaveGameObject(UAllRecordData_OpRecord::StaticClass()));
	UGameplayStatics::SaveGameToSlot(mOpRecordFileData, fileName, DataCenter::DATA_USER_INDEX);

	auto fileNameItem = NewObject<UAllRecordData_fileNameItem>();
	fileNameItem->fileName = fileName;
	fileNameItem->nTimeStamp = nTimeStamp;
	fileNameItem->nGameMode = RecordStepDataHandler::GetSingleton()->data->nGameMode;
	fileNameItem->bFinish = RecordStepDataHandler::GetSingleton()->data->bWin;
	this->data->tableFileNameItem.Insert(fileNameItem, 0);

	while (this->data->tableFileNameItem.Num() > 100)
	{
		auto mItem = this->data->tableFileNameItem.Pop();
		fileName = mItem->fileName;
		UGameplayStatics::DeleteGameInSlot(fileName, DataCenter::DATA_USER_INDEX);
	}

	UE_LOG(LogTemp, Log, TEXT("当前记录局数: %d"), this->data->tableFileNameItem.Num());
}

URecordStepData* AllRecordDataHandler::GetDataByFileItem(UAllRecordData_fileNameItem* fileNameItem)
{
	FString fileName = fileNameItem->fileName;
	USaveGame* mOpData = UGameplayStatics::LoadGameFromSlot(fileName, DataCenter::DATA_USER_INDEX);
	if (mOpData)
	{
		return Cast<UAllRecordData_OpRecord>(mOpData)->tableOpRecord;
	}
	return nullptr;
}

URecordStepData* AllRecordDataHandler::RemoveAndGetDataByFileItem(UAllRecordData_fileNameItem* fileNameItem)
{
	int32 nRemoveIndex = -1;
	for (int i = 0; i < this->data->tableFileNameItem.Num(); i++)
	{
		UAllRecordData_fileNameItem* mItem = this->data->tableFileNameItem[i];
		if (mItem->nTimeStamp == fileNameItem->nTimeStamp)
		{
			nRemoveIndex = i;
			break;
		}
	}

	ensureMsgf(nRemoveIndex > 0, TEXT("RemoveAndGetDataByFileItem Error: %d"), nRemoveIndex);
	this->data->tableFileNameItem.RemoveAt(nRemoveIndex);

	FString fileName = fileNameItem->fileName;
	USaveGame* mOpData = UGameplayStatics::LoadGameFromSlot(fileName, DataCenter::DATA_USER_INDEX);
	if (mOpData)
	{
		return Cast<UAllRecordData_OpRecord>(mOpData)->tableOpRecord;
	}
	return nullptr;
}

URecordStepData* AllRecordDataHandler::RemoveAndGetLastDifferentGameModeRecordTable(int32 nNowGameMode)
{
	int32 nRemoveIndex = -1;
	for (int i = 0; i < this->data->tableFileNameItem.Num(); i++)
	{
		auto mItem = this->data->tableFileNameItem[i];
		if (mItem->nGameMode == nNowGameMode && mItem->bFinish == false)
		{
			nRemoveIndex = i;
			break;
		}
	}

	if (nRemoveIndex > 0)
	{
		auto fileNameItem = TArrayExtentions::Remove(this->data->tableFileNameItem, nRemoveIndex);
		FString fileName = fileNameItem->fileName;
		USaveGame* mOpData = UGameplayStatics::LoadGameFromSlot(fileName, DataCenter::DATA_USER_INDEX);
		if (mOpData)
		{
			return Cast<UAllRecordData_OpRecord>(mOpData)->tableOpRecord;
		}
	}

	return nullptr;
}

//
void AllRecordDataHandler::ClearAllTripGame()
{
	for (int i = this->data->tableFileNameItem.Num() - 1; i >= 0; i--)
	{
		auto mItem = this->data->tableFileNameItem[i];
		if (mItem->nGameMode == SolitaireGameMode::Trip)
		{
			this->data->tableFileNameItem.RemoveAt(i);
			FString fileName = mItem->fileName;
			UGameplayStatics::DeleteGameInSlot(fileName, DataCenter::DATA_USER_INDEX);
		}
	}
}

//
URecordStepData* AllRecordDataHandler::RemoveAndGetTripHalfGame() //残局
{
	int32 nRemoveIndex = -1;
	for (int i = this->data->tableFileNameItem.Num(); i >= 3; i--)
	{
		auto mItem = this->data->tableFileNameItem[i];
		if (mItem->nGameMode == SolitaireGameMode::Normal && mItem->bFinish == false)
		{
			nRemoveIndex = i;
			break;
		}
	}

	if (nRemoveIndex > 0)
	{
		auto fileNameItem = TArrayExtentions::Remove(this->data->tableFileNameItem, nRemoveIndex);
		FString fileName = fileNameItem->fileName;
		USaveGame* mOpData = UGameplayStatics::LoadGameFromSlot(fileName, DataCenter::DATA_USER_INDEX);
		if (mOpData)
		{
			return Cast<UAllRecordData_OpRecord>(mOpData)->tableOpRecord;
		}
	}
	return nullptr;
}