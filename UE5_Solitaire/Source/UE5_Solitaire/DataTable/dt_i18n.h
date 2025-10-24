// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "dt_i18n.generated.h"

USTRUCT()
struct FTableRow_i18n : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) FString key;
    UPROPERTY(EditAnywhere) FString English;
    UPROPERTY(EditAnywhere) FString Chinese;
    UPROPERTY(EditAnywhere) FString Japanese;
    UPROPERTY(EditAnywhere) FString Hindi;
    UPROPERTY(EditAnywhere) FString Arabic;
    UPROPERTY(EditAnywhere) FString French;
    UPROPERTY(EditAnywhere) FString Spanish;
    UPROPERTY(EditAnywhere) FString Russian;
};

UCLASS()
class UE5_SOLITAIRE_API UDataTable_i18n : public UDataTable
{
    GENERATED_BODY()

    UDataTable_i18n()
    {
        RowStruct = FTableRow_i18n::StaticStruct();   // 告诉表用哪一行结构
    }
};