// Fill out your copyright notice in the Description page of Project Settings.

#include "CSVConfigMgr.h"

void CSVConfigMgr::Init()
{
    this->LoadCSV<csv_i18n>("i18n.csv");
    this->LoadCSV<csv_jianhuan_vita>("jianhuan_vita.csv");
    this->LoadCSV<csv_jianhuan_newbie>("jianhuan_vita.csv");
    this->LoadCSV<csv_stagereward>("jianhuan_vita.csv");
    this->LoadCSV<csv_table2element>("jianhuan_vita.csv");
    this->LoadCSV<csv_theme>("jianhuan_vita.csv");
    this->LoadCSV<csv_themeitem>("jianhuan_vita.csv");
}
