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

class KKTypeInnerFunc
{
public:
	static uint16 GetTypeId()
	{
		static uint16 nKKTypeInnerAutoAddID;
		check(nKKTypeInnerAutoAddID < UINT16_MAX)
		return ++nKKTypeInnerAutoAddID;
	}
private:
	KKTypeInnerFunc() = delete;
	~KKTypeInnerFunc() = delete;
	KKTypeInnerFunc(const KKTypeInnerFunc&) = delete;
	KKTypeInnerFunc& operator=(const KKTypeInnerFunc&) = delete;
	KKTypeInnerFunc(KKTypeInnerFunc&&) = delete;
	KKTypeInnerFunc& operator=(KKTypeInnerFunc&&) = delete;
};

template<typename T>
class KKTypeTBase
{
public:
	static uint16 GetTypeId()
	{
		static uint16 nKKTypeId;
		if (nKKTypeId == 0)
		{
			nKKTypeId = KKTypeInnerFunc::GetTypeId();
		}
		return nKKTypeId;
	}
};
