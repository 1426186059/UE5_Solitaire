// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_jianhuan_newbie.generated.h"

USTRUCT()
struct FDT_jianhuan_newbie : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) int32 index = {};
    UPROPERTY(EditAnywhere) FString sid = {};
};