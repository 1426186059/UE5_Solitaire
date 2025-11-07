// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class KKInstanceIdBase
{
	uint64 nInstanceId = 0;
public:
	uint64 GetInstanceId()
	{
		if (nInstanceId == 0)
		{
			static uint64 InnerAutoAddID;
			nInstanceId = ++InnerAutoAddID;
		}
		return this->nInstanceId;
	}
};

template<typename T>
class KKTypeTBase
{
public:
	static uint64 GetTypeId()
	{
		static uint64 nKKTypeId;
		if (nKKTypeId == 0)
		{
			static uint64 nKKTypeInnerAutoAddID;
			nKKTypeId = ++nKKTypeInnerAutoAddID;
		}
		return nKKTypeId;
	}
};
