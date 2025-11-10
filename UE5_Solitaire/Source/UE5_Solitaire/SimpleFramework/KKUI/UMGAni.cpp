#include "UMGAni.h"

void UMGAni::PlayShowRightToLeftAni(UUserWidget* mUIRoot, bool bShow, TFunction<void()> finishFunc)
{
    float fScreenWidth = 750;
    auto mAniObj = mUIRoot->GetWidgetFromName("aniObj");
    if (bShow)
    {
        mUIRoot->SetVisibility(ESlateVisibility::Visible);
        UMGHelper::SetSlotPos(mAniObj, FVector2D(fScreenWidth, 0));
        KKTweenExtentions::UMG_MoveLocal_RenderPosX(mAniObj, 0, 0.45, KKTween::EaseType::easeOutSine)->SetOnCompleteFunc([=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });
    }
    else
    {
        UMGHelper::SetSlotPos(mAniObj, FVector2D(0, 0));
        KKTweenExtentions::UMG_MoveLocal_RenderPosX(mAniObj, fScreenWidth + 200, 0.45, KKTween::EaseType::easeOutSine)->SetOnCompleteFunc([=]()
            {
                mUIRoot->SetVisibility(ESlateVisibility::Hidden);
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });
    }
}

void UMGAni::PlayShowScaleAni(UUserWidget* mUIRoot, bool bShow, TFunction<void()> finishFunc)
{
    auto mAniObj = mUIRoot->GetWidgetFromName("aniObj");
    if (bShow)
    {
        mUIRoot->SetVisibility(ESlateVisibility::Visible);
        mAniObj->SetRenderScale(FVector2D::ZeroVector);
        auto toScale = FVector2D(1, 1);
        KKTweenExtentions::UMG_Scale(mAniObj, toScale, 0.45, KKTween::EaseType::easeOutSine)->SetOnCompleteFunc([=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });
    }
    else
    {
        KKTweenExtentions::UMG_Scale(mAniObj, FVector2D::ZeroVector, 0.45, KKTween::EaseType::easeOutSine)->SetOnCompleteFunc([=]()
            {
                mUIRoot->SetVisibility(ESlateVisibility::Hidden);
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });
    }
}

void UMGAni::PlayShowAlphaAni(UUserWidget* mUIRoot, bool bShow, TFunction<void()> finishFunc)
{
    auto mAniObj = mUIRoot->GetWidgetFromName("aniObj");
    if (bShow)
    {
        mUIRoot->SetVisibility(ESlateVisibility::Visible);
        mAniObj->SetRenderOpacity(0);
        KKTweenExtentions::UMG_Opacity(mAniObj, 1.0, 0.25, KKTween::EaseType::linear)->SetOnCompleteFunc([=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });
    }
    else
    {
        mAniObj->SetRenderOpacity(1);
        KKTweenExtentions::UMG_Opacity(mAniObj, 0, 0.25, KKTween::EaseType::linear)->SetOnCompleteFunc([=]()
            {
                mUIRoot->SetVisibility(ESlateVisibility::Hidden);
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });
    }
}

void UMGAni::PlayShowDownToUpAni(UUserWidget* mUIRoot, bool bShow, TFunction<void()> finishFunc)
{
    float fScreenHeight = 500;
    float height = fScreenHeight + 300;
    auto mAniObj = mUIRoot->GetWidgetFromName("aniObj");

    if (bShow)
    {
        mUIRoot->SetVisibility(ESlateVisibility::Visible);
        UMGHelper::SetSlotPos(mAniObj, FVector2D(0, height));
        mAniObj->SetRenderOpacity(0);

        KKTweenExtentions::UMG_MoveLocal_RenderPosY(mAniObj, 0.0, 0.45f, KKTween::EaseType::easeOutSine)->SetOnCompleteFunc([=]()
            {
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });

        KKTweenExtentions::UMG_Opacity(mAniObj, 1.0, 0.3, KKTween::EaseType::easeOutSine);
    }
    else
    {
        UMGHelper::SetSlotPos(mAniObj, FVector2D(0, 0));
        KKTweenExtentions::UMG_MoveLocal_RenderPosY(mAniObj, height, 0.65f, KKTween::EaseType::easeInSine)->SetOnCompleteFunc([=]()
            {
                mUIRoot->SetVisibility(ESlateVisibility::Hidden);
                if (finishFunc.IsSet())
                {
                    finishFunc();
                }
            });
        KKTweenExtentions::UMG_Opacity(mAniObj, 0, 0.3, KKTween::EaseType::easeInSine);
    }
}


