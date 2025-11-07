// Fill out your copyright notice in the Description page of Project Settings.

#include "DTMgr.h"

TMap<TSubclassOf<UDataTable>, TWeakObjectPtr<UDataTable>> DTMgr::mConfigDic = {};

void DTMgr::Init()
{
    //this->LoadTable<UDataTable_jianhuan_vita>(TEXT("/Game/ResourceABs/MainScene/Config/DT/jianhuan_vita.jianhuan_vita"));
    //this->LoadTable<UDataTable_i18n>(TEXT("/Game/ResourceABs/MainScene/Config/DT/i18n.i18n"));
}
