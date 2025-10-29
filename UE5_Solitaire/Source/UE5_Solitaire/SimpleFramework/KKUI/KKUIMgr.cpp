#include "KKUIMgr.h"

AKKUIMgr::AKKUIMgr()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AKKUIMgr::Init()
{
    GetRootWidget()->Init();
}

