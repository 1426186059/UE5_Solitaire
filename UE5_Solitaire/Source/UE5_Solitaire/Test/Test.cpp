// Fill out your copyright notice in the Description page of Project Settings.


#include "Test.h"

Test::Test()
{
}

Test::~Test()
{
}

void Test::Do()
{
#ifdef WITH_EDITOR
	FString Line1(",,,");
	TArray<FString> Cols1;
	Line1.ParseIntoArray(Cols1, TEXT(","), false);
	UE_LOG(LogTemp, Error, TEXT("Test Line.ParseIntoArray: %d"), Cols1.Num());

	FString Line2(",");
	TArray<FString> Cols2;
	Line2.ParseIntoArray(Cols2, TEXT(","), false);
	UE_LOG(LogTemp, Error, TEXT("Test Line.ParseIntoArray: %d"), Cols2.Num());


	//FString A;
	//A.TrimStart().RightChopInline(1);
	//A.TrimEnd().RightChopInline(1);

	KKTypeTestClass1 mClass1 = {};
	KKTypeTestClass2 mClass2 = {};
	KKTypeTestClass3 mClass3 = {};

	UE_LOG(
		LogTemp, 
		Error, 
		TEXT("Test KKType Test: %d,%d, %d,%d, %d,%d, %d,%d"), 
		KKTypeTestClass1::GetTypeId(), KKTypeTestClass1::GetTypeId(),
		KKTypeTestClass2::GetTypeId(), KKTypeTestClass2::GetTypeId(),
		KKTypeTestClass3::GetTypeId(), KKTypeTestClass3::GetTypeId(),
		KKTypeTestClass4::GetTypeId(), KKTypeTestClass4::GetTypeId()
	);
#endif
}
