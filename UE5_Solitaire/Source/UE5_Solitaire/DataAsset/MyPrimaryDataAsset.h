// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UE5_SOLITAIRE_API UMyPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("UIAsset", GetFName());
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "UI"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "UI"))
	TSoftObjectPtr<UTexture2D> AtlasTexture;   // 如果是 AtlasGroup 就换成 UAtlasGroup
};
