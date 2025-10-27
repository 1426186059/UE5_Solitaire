#include "StatisticDataHandler.h"

void StatisticDataHandler::Init()
{
	UStatisticData* mmmData = DataCenter::GetSingleton()->data->ThemeData;
	if (mmmData == nullptr)
	{
		this->data = NewObject<UStatisticData>();
		DataCenter::GetSingleton()->data->StatisticData = this->data;
	}
	else
	{
		this->data = mmmData;
	}
}
