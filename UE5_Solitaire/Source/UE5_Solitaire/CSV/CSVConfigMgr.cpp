// Fill out your copyright notice in the Description page of Project Settings.

#include "CSVConfigMgr.h"

void CSVConfigMgr::Init()
{
    this->LoadCSV<csv_i18n>("i18n.csv");
    this->LoadCSV<csv_jianhuan_vita>("jianhuan_vita.csv");
    this->LoadCSV<csv_jianhuan_newbie>("jianhuan_newbie.csv");
    this->LoadCSV<csv_stagereward>("1105stagereward.csv");
    this->LoadCSV<csv_table2element>("1301table2element.csv");
    this->LoadCSV<csv_theme>("1203theme.csv");
    this->LoadCSV<csv_themeitem>("1104themeitem.csv");
}
