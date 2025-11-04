#include "DataCenter.h"

const FString DataCenter::DATA_SLOT_NAME = "SolitaireData";
const int32 DataCenter::DATA_USER_INDEX = 0;

DataCenter::~DataCenter()
{
	if (mSaveDataHandle.IsValid())
	{
		mSaveDataHandle.Reset();
	}
}

void DataCenter::Init(InitFinishFunc func)
{
	this->mInitFinishFunc = func;
	this->LoadData();

	mSaveDataHandle = AKKDBSaveMgr::GetSingleton()->GetEventList()->AddLambda([this]()
		{
			this->SaveData();
		});
}

UGameData* DataCenter::GetData()
{
	return this->data.Get();
}

void DataCenter::LoadData(bool bSync)
{
	if (bSync)
	{
		USaveGame* SaveGameInstance = UGameplayStatics::LoadGameFromSlot(DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX);
		if (SaveGameInstance != NULL)
		{
			this->data = TStrongObjectPtr<UGameData>(Cast<UGameData>(SaveGameInstance));
		}
		else
		{
			this->data = TStrongObjectPtr<UGameData>(Cast<UGameData>(UGameplayStatics::CreateSaveGameObject(UGameData::StaticClass())));
		}
	}
	else
	{
		FAsyncLoadGameFromSlotDelegate LoadedDelegate;
		LoadedDelegate.BindRaw(this, &DataCenter::OnLoadDataComplete);
		UGameplayStatics::AsyncLoadGameFromSlot(DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX, LoadedDelegate);
	}

	ensureMsgf(this->data.IsValid(), TEXT("DataCenter::LoadData: Error "));
}

void DataCenter::OnLoadDataComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGameInstance)
{
	if (SaveGameInstance != NULL)
	{
		this->data = TStrongObjectPtr<UGameData>(Cast<UGameData>(SaveGameInstance));
	}
	else
	{
		this->data = TStrongObjectPtr<UGameData>(Cast<UGameData>(UGameplayStatics::CreateSaveGameObject(UGameData::StaticClass())));
	}

	if (this->mInitFinishFunc != NULL)
	{
		this->mInitFinishFunc();
	}
}

void DataCenter::SaveData(bool bSync)
{
	if (bSync)
	{
		UGameplayStatics::SaveGameToSlot(this->GetData(), DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX);
		UE_LOG(LogTemp, Log, TEXT("SaveData completed"));
	}
	else
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindRaw(this, &DataCenter::OnSaveDataComplete);
		UGameplayStatics::AsyncSaveGameToSlot(this->GetData(), DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX, SavedDelegate);
	}
}

void DataCenter::OnSaveDataComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("SaveData Async completed"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SaveData Async failed."));
	}
}


void DataCenter::AddMagicWandCount(int32 nAddCount)
{
	GetData()->nMagicWandCount += nAddCount;
	GetData()->nMagicWandCount = FMath::Clamp(this->data->nMagicWandCount, 0, 9);
}

void DataCenter::AddTotalGameCount()
{
	GetData()->nTotalGameCount++;
}

void DataCenter::AddTotalWinGameCount()
{
	GetData()->nTotalWinGameCount++;
}

void DataCenter::AddCoinCount(int32 nCointCount)
{
	GetData()->nCoinCount += nCointCount;
}

void DataCenter::SetDrawCount(int32 nDrawCount)
{
	GetData()->nDrawCount = nDrawCount;
}

void DataCenter::AddGameLevel()
{
	GetData()->nGameLevel++;
}

void DataCenter::AddNomalModeTotalWinCount()
{
	GetData()->nNomalModeTotalWinCount++;
}

void DataCenter::AddDifficultLayerWinResult(bool bWin)
{
	if (bWin)
	{
		GetData()->nDifficultLayer_ContinueLoseCount = 0;
		GetData()->nDifficultLayer_ContinueWinCount++;
		if (GetData()->nDifficultLayer_ContinueWinCount > 3)
		{
			GetData()->nDifficultLayer_ContinueWinCount = 0;
			GetData()->nDifficultLayer++;
		}
	}
	else
	{
		GetData()->nDifficultLayer_ContinueWinCount = 0;
		GetData()->nDifficultLayer_ContinueLoseCount++;
		if (GetData()->nDifficultLayer_ContinueLoseCount > 3)
		{
			GetData()->nDifficultLayer_ContinueLoseCount = 0;
			GetData()->nDifficultLayer--;
		}
	}
	GetData()->nDifficultLayer = FMath::Clamp(this->data->nDifficultLayer, 1, 10);
}

void DataCenter::SetLanguageName(FString langName)
{
	GetData()->langName = langName;
	//CS.LanguageManager.Instance : OnSelectLanguage(self.data.langName)
}

void DataCenter::SetUIStype(int32 nUIStyle)
{
	GetData()->nUIStyle = nUIStyle;
	//CS.UIStyleSwitchMgr.Instance : DoSwitch(self.data.nUIStyle)
}

void DataCenter::SetSoundOpen(bool bTrue)
{
	GetData()->bOpenSound = bTrue;
}

void DataCenter::SetClickToMove(bool bTrue)
{
	GetData()->bClickToMove = bTrue;
}

void DataCenter::SetAutoHint(bool bTrue)
{
	GetData()->bAutoHint = bTrue;
}

void DataCenter::SetAutoComplete(bool bTrue)
{
	GetData()->bAutoComplete = bTrue;
}

void DataCenter::SetFastGame(bool bTrue)
{
	GetData()->bFastGame = bTrue;
}

void DataCenter::SetIQMode(bool bTrue)
{
	GetData()->bIQMode = bTrue;
}

void DataCenter::AddIQValue(int32 nIQValue)
{
	GetData()->nIQValue += nIQValue;
	GetData()->nIQValue = FMath::Clamp(this->data->nIQValue, 75, 300);
}

void DataCenter::UpdateMusic(int32 nMusicIndex)
{
	GetData()->nMusicIndex = nMusicIndex;
}

void DataCenter::SetLeftHandMode(bool bLeftHandMode)
{
	GetData()->bLeftHandMode = bLeftHandMode;
}

