// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_jianhuan_vita.generated.h"

USTRUCT()
struct F_jianhuan_vita_TableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) FString id;
    UPROPERTY(EditAnywhere) int32 layer;
    UPROPERTY(EditAnywhere) int32 qid;
    UPROPERTY(EditAnywhere) int32 step;
    UPROPERTY(EditAnywhere) int32 available;
    UPROPERTY(EditAnywhere) FString jianhuanstr;
    UPROPERTY(EditAnywhere) FString sourcefrom;
    UPROPERTY(EditAnywhere) int32 fromid;
    UPROPERTY(EditAnywhere) FString sourcestr;
};

UCLASS()
class UE5_SOLITAIRE_API U_jianhuan_vita_DataTable : public UDataTable
{
    GENERATED_BODY()

    U_jianhuan_vita_DataTable()
    {
        RowStruct = F_jianhuan_vita_TableRow::StaticStruct();   // 告诉表用哪一行结构
    }
};