#include "AnimationView2_Default_Widget.h"
#include "UE5_Solitaire/UI/GameWinAniMgr.h"
#include "UE5_Solitaire/GameLogic/AudioHandler.h"

void UAnimationView2_Default_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    this->UpdateAllAniEntry(InDeltaTime);
}

void UAnimationView2_Default_Widget::Init()
{
    if (this->bInit) return;
    this->bInit = true;

    FVector2D RootSize = UMGHelper::GetUMGRootSzie(this);
    UE_LOG(LogTemp, Log, TEXT("UAnimationView2_Default_Widget RootSize: %s"), *RootSize.ToString());

    this->maxY = RootSize.Y / 2.0f - 100;
    this->minY = -RootSize.Y / 2.0f;
    this->maxX = RootSize.X / 2.0f + 150;
    this->minX = -RootSize.X / 2.0f - 150;
    //this->maxWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
    //this->minWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
    this->ItemParent = Cast<UCanvasPanel>(this->GetWidgetFromName("ItemParent"));
    UMGHelper::RemoveAllChildren(this->ItemParent);

    this->skipNode = this->GetWidgetFromName("SkipBtn");
    auto skipBtn = Cast<UButton>(this->skipNode);
    skipBtn->OnClicked.AddDynamic(this, &UAnimationView2_Default_Widget::onClick_Skip);
}

void UAnimationView2_Default_Widget::CheckFirstLayoutOkToShow()
{
    if (orFirstLayoutFinish())
    {
        if (this->bShowUI)
        {
            this->SetVisibility(ESlateVisibility::Visible);
            this->Init();
            this->PlayAni();
        }
    }
}

void UAnimationView2_Default_Widget::SetMgr(UGameWinAniMgr* mgr)
{
    this->mMgr = mgr;
}

void UAnimationView2_Default_Widget::PlayAni()
{
    TArray<int32> table4AColor = this->mMgr->GetTableA4Color();
    TArray<FVector2D> table4AWorldPos = this->mMgr->GetTableA4WorldPos();

    for (int i = 0; i < table4AWorldPos.Num(); i++)
    {
        auto pos = UMGHelper::GetRLocalPos(this->ItemParent, table4AWorldPos[i]);
        UE_LOG(LogTemp, Log, TEXT("UAnimationView2_Default_Widget table4AWorldPos: %s"), *pos.ToString());
        table4AWorldPos[i] = pos;
    }
    
    for (int i = 0; i < table4AColor.Num(); i++)
    {
        int nColor = table4AColor[i];
        FVector2D beginPos = table4AWorldPos[i];
        for (int j = 13; j >= 1; j--)
        {
            this->CreateAniEntry(i, nColor, j, beginPos, i * 0.5f + (13 - j) * 0.1f);
        }
    }
}

void UAnimationView2_Default_Widget::CreateAniEntry(int nColIndex, int nColor, int nDigitId, FVector2D beginPos, float delay)
{
    int nPokerId = CardHandler::GetSingleton()->GetPokerId(nDigitId, nColor);
    auto mPokerItem = this->GetPoolCard();
    mPokerItem->InitPokerId(nPokerId);
    UMGHelper::SetSlotPos(mPokerItem, beginPos);
    UMGHelper::SetChildFirstZOrder(mPokerItem);

    auto mEntry = AnimationEntity::Create(nDigitId, nColor);
    mEntry->open = true;
    mEntry->trigger = false;
    mEntry->triggerDelay = delay;
    mEntry->nColIndex = nColIndex;
    mEntry->btoRight = AnimationEntity::toRight(nColIndex);
    mEntry->beginPos = beginPos;
    mEntry->nowPos = beginPos;
    mEntry->mPokerWidget = mPokerItem;
    mEntry->minY = this->minY;
    mEntry->vx = AnimationEntity::randomVx();
    mEntry->vx_a = 0;
    mEntry->vy = AnimationEntity::randomVy();
    mEntry->vy_a = AnimationEntity::randomVy_a();

    if (!mEntry->btoRight)
    {
        mEntry->vx *= -1;
    }
    
    this->animationEntitys.Add(mEntry);
}

//检查是否最后一个队列中的最后一个，标志动画结束。   
UPokerAnimationItemW* UAnimationView2_Default_Widget::GetPoolCard()
{
    auto startNode = this->mMgr->mCardItemPool->Pop();
    UMGHelper::SetParent(startNode, this->ItemParent);
    UMGHelper::SetSlotAnchor(startNode, FAnchors(0.5));
    UMGHelper::SetSlotAlignment(startNode, FVector2D(0.5));
    UMGHelper::SetSlotPos(startNode, FVector2D(0));
    UMGHelper::SetSlotSize(startNode, FVector2D(0));
    this->allNodes.Add(startNode);
    return startNode;
}

void UAnimationView2_Default_Widget::RecyclePoolCard(UPokerAnimationItemW* mCard)
{
    this->mMgr->mCardItemPool->Recycle(mCard);
}

void UAnimationView2_Default_Widget::UpdateAniEntry(TSharedPtr<AnimationEntity> mEntry, float dt)
{
    if (!mEntry->open)
    {
        return;
    }

    if (!mEntry->trigger)
    {
        mEntry->triggerDelay -= dt;
        if (mEntry->triggerDelay <= 0)
        {
            mEntry->trigger = true;;
        }
        return;
    }
    
    // 没有节点的时候，不更新。
    if (mEntry->mPokerWidget == nullptr)
    {
        return;
    }
    
    FVector2D startPt = mEntry->nowPos;
    float toRight = mEntry->btoRight;
    float vx_a = mEntry->vx_a;
    float vy_a = mEntry->vy_a;

    FVector2D nowPt = FVector2D(0, 0);

    // 现在速度
    mEntry->vx += vx_a * dt;
    mEntry->vy += vy_a * dt;

    float vx = mEntry->vx;
    float vy = mEntry->vy;

    nowPt.X = (float)(startPt.X + vx * dt + 0.5f * vx_a * dt * dt);
    nowPt.Y = (float)(startPt.Y + vy * dt + 0.5f * vy_a * dt * dt);

    // 垂直. 小于最低值。
    if (nowPt.Y < mEntry->minY)
    {
        nowPt.Y = mEntry->minY;
        mEntry->vy = 0;
        mEntry->minY += 100;
    }

    if (nowPt.Y > this->maxY)
    {
        nowPt.Y = this->maxY;
        mEntry->vy *= -0.95f;
    }
    
    bool willRemove = false;
    if (nowPt.X < this->minX - CardWidth)
    {
        nowPt.X = this->minX;
        willRemove = true;
    }

    if (nowPt.X > this->maxX + CardWidth)
    {
        nowPt.X = this->maxX;
        mEntry->vx *= -1;
        willRemove = true;
    }

    UMGHelper::SetSlotPos(mEntry->mPokerWidget, nowPt);
    mEntry->nowPos = nowPt;
    
    if (willRemove)
    {
        mEntry->open = false;
        if (mEntry->nDigitId == 6 && mEntry->nColor == 2)
        {
            this->onAnimatinCallBack();
        }

        return;
    }
}

void UAnimationView2_Default_Widget::UpdateAllAniEntry(float dt)
{
    for (int i = 0; i < this->animationEntitys.Num(); i++)
    {
        auto mEntry = this->animationEntitys[i];
        this->UpdateAniEntry(mEntry, dt);
    }
}

void UAnimationView2_Default_Widget::onAnimatinCallBack()
{
    this->DoDestroyAction();
    if (this->callBack.IsSet())
    {
        this->callBack();
        this->callBack.Reset();
    }
}

void UAnimationView2_Default_Widget::DoDestroyAction()
{
    if (this->animationOver)
    {
        return;
    }

    this->animationOver = true;
    this->animationEntitys = {};

    for(auto v : this->allNodes)
    {
        RecyclePoolCard(v);
    }
    this->allNodes = {};
    UMGHelper::DestroyWidget(this);
}

void UAnimationView2_Default_Widget::onClick_Skip()
{
    UE_LOG(LogTemp, Log, TEXT("UAnimationView2_Default_Widget onClick_Skip"));
    this->onAnimatinCallBack();
}