// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_table2element.generated.h"

USTRUCT()
struct FDT_table2element : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere) int32 id = {};
    UPROPERTY(EditAnywhere) int32 tableid = {};
    UPROPERTY(EditAnywhere) int32 paiid = {};
};