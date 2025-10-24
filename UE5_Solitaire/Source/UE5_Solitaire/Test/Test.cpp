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
	FString Line1(",,,");
	TArray<FString> Cols1;
	Line1.ParseIntoArray(Cols1, TEXT(","), false);
	UE_LOG(LogTemp, Error, TEXT("Test Line.ParseIntoArray: %d"), Cols1.Num());

	FString Line2(",");
	TArray<FString> Cols2;
	Line2.ParseIntoArray(Cols2, TEXT(","), false);
	UE_LOG(LogTemp, Error, TEXT("Test Line.ParseIntoArray: %d"), Cols2.Num());
}
