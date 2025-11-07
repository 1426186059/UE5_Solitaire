// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_theme.generated.h"

USTRUCT()
struct FDT_theme : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)  int32 id = {};
    UPROPERTY(EditAnywhere) FString name = {};
    UPROPERTY(EditAnywhere) int32 table = {};
    UPROPERTY(EditAnywhere) int32 front = {};
    UPROPERTY(EditAnywhere) int32 back = {};
    UPROPERTY(EditAnywhere) int32 pai = {};
    UPROPERTY(EditAnywhere) int32 unlocktype = {};
    UPROPERTY(EditAnywhere) int32 unlockcost = {};
    UPROPERTY(EditAnywhere) int32 order = {};
};