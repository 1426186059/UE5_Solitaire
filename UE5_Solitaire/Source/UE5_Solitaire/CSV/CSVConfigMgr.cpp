// Fill out your copyright notice in the Description page of Project Settings.

#include "CSVConfigMgr.h"

void CSVConfigMgr::Init()
{
    this->LoadCSV<CSV_i18n>("i18n.csv");
    this->LoadCSV<CSV_jianhuan_vita>("jianhuan_vita.csv");
}
