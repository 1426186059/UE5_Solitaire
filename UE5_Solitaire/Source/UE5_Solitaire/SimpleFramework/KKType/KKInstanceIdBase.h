// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class KKInstanceIdBase
{
private:
	static uint64 InnerAutoAddID;
	uint64 nInstanceId = 0;
public:
	uint64 GetInstanceId()
	{
		if (nInstanceId == 0)
		{
			nInstanceId = ++InnerAutoAddID;
		}
		return this->nInstanceId;
	}
};

class KKTypeBase
{
protected:
	static uint64 nKKTypeInnerAutoAddID;
};

template<typename T>
class KKTypeTBase : public KKTypeBase
{
	friend class KKTypeBase;
private:
	static uint64 nKKTypeId;
public:
	static uint64 GetTypeId()
	{
		if (nKKTypeId == 0)
		{
			nKKTypeId = ++nKKTypeInnerAutoAddID;
		}
		return nKKTypeId;
	}
};
