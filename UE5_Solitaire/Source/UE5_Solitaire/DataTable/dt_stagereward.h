// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_stagereward.generated.h"

USTRUCT()
struct FTableRow_stagereward : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) int32 id = {};
    UPROPERTY(EditAnywhere) int32 rewardtype = {};
    UPROPERTY(EditAnywhere) int32 rewardid = {};
    UPROPERTY(EditAnywhere) int32 wincount = {};
    UPROPERTY(EditAnywhere) FString imgsrc = {};
};


UCLASS()
class UE5_SOLITAIRE_API UDataTable_stagereward : public UDataTable
{
    GENERATED_BODY()

    UDataTable_stagereward()
    {
        RowStruct = FTableRow_stagereward::StaticStruct();   // 告诉表用哪一行结构
    }
};