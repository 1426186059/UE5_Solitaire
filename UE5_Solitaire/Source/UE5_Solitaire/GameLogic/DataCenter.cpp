#include "DataCenter.h"

const FString DataCenter::DATA_SLOT_NAME = "SolitaireData";
const int32 DataCenter::DATA_USER_INDEX = 0;

void DataCenter::Init(InitFinishFunc func)
{
	this->mInitFinishFunc = func;
	this->LoadData();

	FCoreDelegates::ApplicationWillDeactivateDelegate.AddLambda([=, this]()
		{
			this->OnApplicationPause();
		});

	FCoreDelegates::ApplicationHasReactivatedDelegate.AddLambda([=, this]()
		{
			this->OnApplicationResume();
		});

	FCoreDelegates::OnPreExit.AddRaw(
		this, &DataCenter::OnApplicationQuit);
}

void DataCenter::OnApplicationPause()
{
	UE_LOG(LogTemp, Log, TEXT("==> DataCenter OnApplicationPause"));
	SaveData();
}

void DataCenter::OnApplicationResume()
{
	UE_LOG(LogTemp, Log, TEXT("==> DataCenter OnApplicationResume"));
}

void DataCenter::OnApplicationQuit()
{
	UE_LOG(LogTemp, Log, TEXT("==> DataCenter OnApplicationQuit"));
}

UGameData* DataCenter::GetData()
{
	return this->data;
}

void DataCenter::LoadData(bool bSync)
{
	if (bSync)
	{
		USaveGame* SaveGameInstance = UGameplayStatics::LoadGameFromSlot(DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX);
		if (SaveGameInstance != NULL)
		{
			this->data = Cast<UGameData>(SaveGameInstance);
		}
		else
		{
			this->data = Cast<UGameData>(UGameplayStatics::CreateSaveGameObject(UGameData::StaticClass()));
		}
	}
	else
	{
		FAsyncLoadGameFromSlotDelegate LoadedDelegate;
		LoadedDelegate.BindRaw(this, &DataCenter::OnLoadDataComplete);
		UGameplayStatics::AsyncLoadGameFromSlot(DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX, LoadedDelegate);
	}
}

void DataCenter::OnLoadDataComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGameInstance)
{
	if (SaveGameInstance != NULL)
	{
		this->data = Cast<UGameData>(SaveGameInstance);
	}
	else
	{
		this->data = Cast<UGameData>(UGameplayStatics::CreateSaveGameObject(UGameData::StaticClass()));
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
		UGameplayStatics::SaveGameToSlot(this->data, DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX);
	}
	else
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindRaw(this, &DataCenter::OnSaveDataComplete);
		UGameplayStatics::AsyncSaveGameToSlot(this->data, DataCenter::DATA_SLOT_NAME, DataCenter::DATA_USER_INDEX, SavedDelegate);
	}
}

void DataCenter::OnSaveDataComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("SaveGameAsync completed successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SaveGameAsync failed."));
	}
}


void DataCenter::AddMagicWandCount(int32 nAddCount)
{
	this->data->nMagicWandCount = this->data->nMagicWandCount + nAddCount;
	this->data->nMagicWandCount = FMath::Clamp(this->data->nMagicWandCount, 0, 9);
}

void DataCenter::AddTotalGameCount()
{
	this->data->nTotalGameCount = this->data->nTotalGameCount + 1;
}

void DataCenter::AddTotalWinGameCount()
{
	this->data->nTotalWinGameCount = this->data->nTotalWinGameCount + 1;
}

void DataCenter::AddCoinCount(int32 nCointCount)
{
	this->data->nCoinCount = this->data->nCoinCount + nCointCount;
}

void DataCenter::SetDrawCount(int32 nDrawCount)
{
	this->data->nDrawCount = nDrawCount;
}

void DataCenter::AddGameLevel()
{
	this->data->nGameLevel = this->data->nGameLevel + 1;
}

void DataCenter::AddNomalModeTotalWinCount()
{
	this->data->nNomalModeTotalWinCount = this->data->nNomalModeTotalWinCount + 1;
}

void DataCenter::AddDifficultLayerWinResult(bool bWin)
{
	if (bWin)
	{
		this->data->nDifficultLayer_ContinueLoseCount = 0;
		this->data->nDifficultLayer_ContinueWinCount = this->data->nDifficultLayer_ContinueWinCount + 1;
		if (this->data->nDifficultLayer_ContinueWinCount > 3)
		{
			this->data->nDifficultLayer_ContinueWinCount = 0;
			this->data->nDifficultLayer = this->data->nDifficultLayer + 1;
		}
	}
	else
	{
		this->data->nDifficultLayer_ContinueWinCount = 0;
		this->data->nDifficultLayer_ContinueLoseCount = this->data->nDifficultLayer_ContinueLoseCount + 1;
		if (this->data->nDifficultLayer_ContinueLoseCount > 3)
		{
			this->data->nDifficultLayer_ContinueLoseCount = 0;
			this->data->nDifficultLayer = this->data->nDifficultLayer - 1;
		}
	}
	this->data->nDifficultLayer = FMath::Clamp(this->data->nDifficultLayer, 1, 10);
}

void DataCenter::SetLanguageName(FString langName)
{
	this->data->langName = langName;
	//CS.LanguageManager.Instance : OnSelectLanguage(self.data.langName)
}

void DataCenter::SetUIStype(int32 nUIStyle)
{
	this->data->nUIStyle = nUIStyle;
	//CS.UIStyleSwitchMgr.Instance : DoSwitch(self.data.nUIStyle)
}

void DataCenter::SetSoundOpen(bool bTrue)
{
	this->data->bOpenSound = bTrue;
}

void DataCenter::SetClickToMove(bool bTrue)
{
	this->data->bClickToMove = bTrue;
}

void DataCenter::SetAutoHint(bool bTrue)
{
	this->data->bAutoHint = bTrue;
}

void DataCenter::SetAutoComplete(bool bTrue)
{
	this->data->bAutoComplete = bTrue;
}

void DataCenter::SetFastGame(bool bTrue)
{
	this->data->bFastGame = bTrue;
}

void DataCenter::SetIQMode(bool bTrue)
{
	this->data->bIQMode = bTrue;
}

void DataCenter::AddIQValue(int32 nIQValue)
{
	this->data->nIQValue = this->data->nIQValue + nIQValue;
	this->data->nIQValue = FMath::Clamp(this->data->nIQValue, 75, 300);
}

void DataCenter::UpdateMusic(int32 nMusicIndex)
{
	this->data->nMusicIndex = nMusicIndex;
}

void DataCenter::SetLeftHandMode(bool bLeftHandMode)
{
	this->data->bLeftHandMode = bLeftHandMode;
}

