// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE5_Solitaire/SimpleFramework/KKSingleton.h"

#include "dt_i18n.h"
#include "dt_jianhuan_vita.h"

class UE5_SOLITAIRE_API DTMgr : public KKSingleton<DTMgr>
{
    friend class KKSingleton<DTMgr>;
public:
    void Init();

private:
    DTMgr() = default;
    ~DTMgr() = default;
    DTMgr(const DTMgr&) = delete;
    DTMgr& operator=(const DTMgr&) = delete;
};
