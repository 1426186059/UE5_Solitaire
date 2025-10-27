#include "ThemeDataHandler.h"

void ThemeDataHandler::Init()
{
	UThemeData* mmmData = DataCenter::GetSingleton()->data->ThemeData;
	if (mmmData == nullptr)
	{
		this->data = NewObject<UThemeData>();
		DataCenter::GetSingleton()->data->ThemeData = this->data;
	}
	else
	{
		this->data = mmmData;
	}
}
