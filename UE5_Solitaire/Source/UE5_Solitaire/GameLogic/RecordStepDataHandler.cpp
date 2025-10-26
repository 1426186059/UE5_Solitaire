#include "RecordStepDataHandler.h"

void RecordStepDataHandler::Init()
{
	URecordStepDataHandlerData* mmmData = DataCenter::GetSingleton()->data->tableOpRecord;
	if (mmmData == nullptr)
	{
		this->data = NewObject<URecordStepDataHandlerData>();
		DataCenter::GetSingleton()->data->tableOpRecord = this->data;
	}
	else
	{
		this->data = mmmData;
	}
}
