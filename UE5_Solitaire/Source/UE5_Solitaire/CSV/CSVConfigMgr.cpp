// Fill out your copyright notice in the Description page of Project Settings.

#include "CSVConfigMgr.h"

void CSVConfigMgr::Init()
{
    this->mCSV_i18n = this->LoadCSV<CSV_i18n>("i18n.csv");
    this->mCSV_jianhuan_vita = this->LoadCSV<CSV_jianhuan_vita>("jianhuan_vita.csv");
}
