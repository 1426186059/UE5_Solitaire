// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_themeitem.generated.h"

USTRUCT()
struct FDT_themeitem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) int32 id = {};
    UPROPERTY(EditAnywhere) int32 internalid = {};
    UPROPERTY(EditAnywhere) int32 type = {};
    UPROPERTY(EditAnywhere) int32 itemunlock = {};
    UPROPERTY(EditAnywhere) FString thumbsrc = {};
    UPROPERTY(EditAnywhere) int32 download = {};
    UPROPERTY(EditAnywhere) FString dynamic = {};
    UPROPERTY(EditAnywhere) int32 order = {};
};