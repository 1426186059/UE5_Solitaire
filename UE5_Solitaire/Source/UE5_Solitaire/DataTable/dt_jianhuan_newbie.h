// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_jianhuan_newbie.generated.h"

USTRUCT()
struct FTableRow_jianhuan_newbie : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) int32 index = {};
    UPROPERTY(EditAnywhere) FString sid = {};
};

UCLASS()
class UE5_SOLITAIRE_API UDataTable_jianhuan_newbie : public UDataTable
{
    GENERATED_BODY()

    UDataTable_jianhuan_newbie()
    {
        RowStruct = FTableRow_jianhuan_newbie::StaticStruct();   // 告诉表用哪一行结构
    }
};
