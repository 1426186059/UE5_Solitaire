// Fill out your copyright notice in the Description page of Project Settings.

#include "DTMgr.h"

void ADTMgr::Init()
{
    this->LoadTable<FDT_jianhuan_vita>(TEXT("jianhuan_vita"));
    this->LoadTable<FDT_i18n>(TEXT("i18n"));
    this->LoadTable<FDT_themeitem>(TEXT("1104themeitem"));;
    this->LoadTable<FDT_stagereward>(TEXT("1105stagereward"));
    this->LoadTable<FDT_theme>(TEXT("1203theme"));
    this->LoadTable<FDT_table2element>(TEXT("1301table2element"));
    this->LoadTable<FDT_jianhuan_newbie>(TEXT("jianhuan_newbie"));
}
