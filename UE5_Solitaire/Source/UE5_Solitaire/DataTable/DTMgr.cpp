// Fill out your copyright notice in the Description page of Project Settings.

#include "DTMgr.h"

void ADTMgr::Init()
{
    this->LoadTable<UDataTable_jianhuan_vita>(TEXT("jianhuan_vita"));
    this->LoadTable<UDataTable_i18n>(TEXT("i18n"));
    this->LoadTable<UDataTable_themeitem>(TEXT("1104themeitem"));;
    this->LoadTable<UDataTable_stagereward>(TEXT("1105stagereward"));
    this->LoadTable<UDataTable_theme>(TEXT("1203theme"));
    this->LoadTable<UDataTable_table2element>(TEXT("1301table2element"));
    this->LoadTable<UDataTable_jianhuan_newbie>(TEXT("jianhuan_newbie"));
}
