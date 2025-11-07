// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_jianhuan_vita.generated.h"

USTRUCT()
struct FTableRow_jianhuan_vita : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) FString sid = {};
    UPROPERTY(EditAnywhere) int32 layer = {};
    UPROPERTY(EditAnywhere) int32 qid = {};
    UPROPERTY(EditAnywhere) int32 step = {};
    UPROPERTY(EditAnywhere) int32 available = {};
    UPROPERTY(EditAnywhere) FString jianhuanstr = {};
    UPROPERTY(EditAnywhere) FString sourcefrom = {};
    UPROPERTY(EditAnywhere) int32 fromid = {};
    UPROPERTY(EditAnywhere) FString sourcestr = {};
};

UCLASS()
class UE5_SOLITAIRE_API UDataTable_jianhuan_vita : public UDataTable
{
    GENERATED_BODY()

    UDataTable_jianhuan_vita()
    {
        RowStruct = FTableRow_jianhuan_vita::StaticStruct();   // 告诉表用哪一行结构
    }
};