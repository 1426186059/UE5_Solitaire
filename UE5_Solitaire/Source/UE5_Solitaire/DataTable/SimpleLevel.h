// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SimpleLevel.generated.h"

USTRUCT()
struct FSimpleLevelTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString sid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 layer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 qid;
};

UCLASS()
class UE5_SOLITAIRE_API USimpleLevelDataTable : public UDataTable
{
    GENERATED_BODY()

    USimpleLevelDataTable()
    {
        RowStruct = FSimpleLevelTableRow::StaticStruct();   // 告诉表用哪一行结构
    }
};