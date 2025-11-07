// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_table2element.generated.h"

USTRUCT()
struct FTableRow_table2element : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere) int32 id = {};
    UPROPERTY(EditAnywhere) int32 tableid = {};
    UPROPERTY(EditAnywhere) int32 paiid = {};
};

UCLASS()
class UE5_SOLITAIRE_API UDataTable_table2element : public UDataTable
{
    GENERATED_BODY()

    UDataTable_table2element()
    {
        RowStruct = FTableRow_table2element::StaticStruct();   // 告诉表用哪一行结构
    }
};