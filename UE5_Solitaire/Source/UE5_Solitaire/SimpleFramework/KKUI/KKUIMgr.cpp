#include "KKUIMgr.h"

void AKKUIMgr::Init()
{
    GetRootWidget()->Init();

    //mWidgetComponent = NewObject<UWidgetComponent>(this);
    //mWidgetComponent->RegisterComponent();
    //mWidgetComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    //// 2. 指定 UMG 类
    //mWidgetComponent->SetWidgetClass(MyWidgetClass);   // 你自己的 UUserWidget 派生类
    //mWidgetComponent->SetDrawSize(FVector2D(1200, 800));

    //// 3. 决定“投影方式”
    //mWidgetComponent->SetGeometryMode(EWidgetGeometryMode::Cylinder); // 透视
    //// 想改回正交就换 -> EWidgetGeometryMode::Orthographic
    //mWidgetComponent->SetWidgetSpace(EWidgetSpace::World); // 透视
}

