// Fill out your copyright notice in the Description page of Project Settings.


#include "KKUIMgr.h"

void AKKUIMgr::Init()
{
    Show<UWUIRoot>("UIRootCWBP");
    Get<UWUIRoot>("UIRootCWBP")->Init();
}

