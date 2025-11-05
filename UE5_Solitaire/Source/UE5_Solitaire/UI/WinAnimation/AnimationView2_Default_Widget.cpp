#include "AnimationView2_Default_Widget.h"
#include "UE5_Solitaire/UI/GameWinAniMgr.h"
#include "UE5_Solitaire/GameLogic/AudioHandler.h"

void UAnimationView2_Default_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

void UAnimationView2_Default_Widget::Init(UGameWinAniMgr* mMgr)
{
    this->mMgr = mMgr;

    FVector2D RootSize = UWidgetLayoutLibrary::GetViewportSize(this);
    this->animationSize = FVector2D(RootSize.X, RootSize.Y);
    this->maxHeight = this->animationSize.Y / 2;
    this->minHeight = -this->animationSize.Y / 2 + 100;
    this->maxWidth = this->animationSize.X / 2 + CardWidth;
    this->minWidth = -this->animationSize.X / 2 - CardWidth;
    //this->maxWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
    //this->minWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
    this->cardsNode = this->GetWidgetFromName("cardsnode");
    this->cardsNode.gameObject.removeAllChildren();
}

void UAnimationView2_Default_Widget::PlayAni()
{
    this->animationOver = false;
    auto table4AColor = this->GetTableA4Color();
    auto table4AWorldPos = this->GetTableA4WorldPos();
    
    FVector2D A = GameTools.WorldToUILocalPos(startPt_w, this->cardsNode);
    float delay = 0.1f;
    //mBlastBgm = UAudioHandler::GetSingleton()->PlaySound(Sounds.blast_bgm);
    for (int i = 0; i < colors.Num(); i++)
    {
        int color = colors[i];
        FVector2D frompt = FVector2D(pt.x + index * (CardWidth - 1) + offset, pt.y, pt.z);
        float delayvalue = 0;
        float delayoffset = 0.1f;
        for (int j = 13; j > 0; j--)
        {
            this->CreateAniEntry(i, pt, index * 0.5f + (13 - j) * 0.1f, color, index, offsetX);
            delayvalue += delayoffset;
        }
    }
}

void UAnimationView2_Default_Widget::CreateAniEntry(int nColIndex, int nColor, int nDigitId, FVector2D beginPos, float delay)
{
    auto node = this->GetPoolCard(pt, color, value, colindex);
    auto startNode = ret.node;

    var entity = AnimationEntity.create(colindex, color, value);
    entity.open = true;
    entity.trigger = false;
    entity.triggerDelay = delay;
    entity.btoRight = AnimationEntity.toRight(colindex);
    entity.startPt = pt;
    entity.nowPt = pt;
    entity.firstNode = startNode;
    entity.maxHeight = this->maxHeight;
    entity.vx = AnimationEntity.randomVx();
    entity.vx_a = 0;
    entity.vy = AnimationEntity.randomVy();
    if (!entity.btoRight)
    {
        entity.vx *= -1;
    }

    entity.vy_a = AnimationEntity.randomVy_a();
    entity.deltTime = 0;
    entity.firstNode = startNode;
    this->animationEntitys.push(entity);
    startNode.transform.SetSiblingIndex(0);
}

    // 检查是否最后一个队列中的最后一个，标志动画结束。   
UWidget* UAnimationView2_Default_Widget::GetPoolCard()
{
    int nodekey = AnimationEntity.getCardId(colorType, value);
    TArray<CardAnimationItem> nodeArrs = this->colNodes_Dic.get(nodekey);
    if (nodeArrs == null)
    {
        nodeArrs = new TArray<CardAnimationItem>();
        this->colNodes_Dic.set(nodekey, nodeArrs);
    }

    bool firstNodeByNewValue = false;
    if (value == 13 && nodeArrs.length == 0)
    {
        firstNodeByNewValue = true;
    }

    AnimationEntity entity = null;
    // 从后往前找，找到第一个。
    for (int index = this->animationEntitys.length - 1; index >= 0; index--)
    {
        AnimationEntity element = this->animationEntitys[index];
        if (element.color == colorType && element.value == value && element.index == colindex)
        {
            entity = element;
            break;
        }
    }

    if (nodeArrs.length >= CardsColTotal)
    {
        if (entity == null)
        {

        }
        else
        {
            if (entity.color == this->colors[this->colors.length - 1] && entity.value == 1)
            {
                this->onAnimatinCallBack();
                this->DoDestroyAction();
            }

        }
        return (null, firstNodeByNewValue);
    }

    auto startNode = ResCenter.Instance.mCardAnimationItemPool.popObj();
    startNode.transform.SetParent(this->cardsNode.transform, false);
    startNode.transform.localPosition = pt;
    nodeArrs.push(startNode);
    startNode.initByNum(value, colorType);

    this->allNodes.push(startNode);
    return (startNode.gameObject, firstNodeByNewValue);
}


void UAnimationView2_Default_Widget::updateAnimation(AnimationEntity entity, float dt)
{
    if (!entity.open)
    {
        return;
    }

    if (entity.trigger)
    {
        // 没有节点的时候，不更新。
        if (entity.firstNode == null)
        {
            return;
        }

        var deltTime = entity.deltTime;
        var startPt = entity.nowPt;
        var firstNode = entity.firstNode;
        var maxHeight = entity.maxHeight;
        var toRight = entity.btoRight;
        var vx_a = entity.vx_a;
        var vy_a = entity.vy_a;

        var nowPt = new FVector2D(0, 0, 0);

        // 匀变速直线运动位移公式：a=dv/dt，
        // 距离 x = v0t+1/2·at^2

        // 现在速度
        entity.vx += vx_a * dt;
        entity.vy += vy_a * dt;

        var vx = entity.vx;
        var vy = entity.vy;

        nowPt.x = (float)(startPt.x + vx * dt + 0.5f * vx_a * dt * dt);
        nowPt.y = (float)(startPt.y + vy * dt + 0.5f * vy_a * dt * dt);
        nowPt.z = startPt.z;

        // 垂直. 小于最低值。
        if (nowPt.y < this->minHeight)
        {
            nowPt.y = this->minHeight;
            entity.vy *= -0.95f;  //转变方向
        }

        if (nowPt.y > entity.maxHeight)
        {
            nowPt.y = entity.maxHeight;
            // vy_a = -vy_a;
            entity.vy = 0;
            entity.maxHeight = entity.maxHeight * 0.8f;
        }


        // X轴移除就消失。
        bool willRemove = false;
        if (nowPt.x < this->minWidth - CardWidth)
        {
            nowPt.x = this->minWidth;
            willRemove = true;
        }

        if (nowPt.x > this->maxWidth + CardWidth)
        {
            nowPt.x = this->maxWidth;
            entity.vx *= -1;
            willRemove = true;


        }

        // 每两帧之间 添加
        entity.checktimes += 1;
        firstNode.transform.localPosition = nowPt;
        entity.nowPt = nowPt;
            
        if (entity.open)
        {
            firstNode.transform.localPosition = nowPt;
            if (willRemove)
            {
                entity.open = false;
                if (entity.value == 6 && entity.index == 2)
                {

                    this->onAnimatinCallBack();
                    this->DoDestroyAction();
                }

                return;
            }
        }
    }
    else
    {

        entity.triggerDelay -= dt;
        if (entity.triggerDelay <= 0)
        {
            entity.trigger = true;
        }
    }
}
    
void UAnimationView2_Default_Widget::Update()
{
    if (gameObject == null) return;

    float dt = Time.deltaTime;
    for (int index = 0; index < this->animationEntitys.length; index++)
    {
        AnimationEntity entity = this->animationEntitys[index];
        this->updateAnimation(entity, dt);
    }
}

void UAnimationView2_Default_Widget::onAnimatinCallBack()
{
    if (this->callBack != null)
    {
        AudioController.Instance.StopSound(Sounds.blast_bgm);
        this->callBack();
        this->callBack = null;
    }
}

void UAnimationView2_Default_Widget::DoDestroyAction()
{
    if (this->animationOver)
    {
        return;
    }

    this->animationOver = true;
    for (int index = 0; index < this->animationEntitys.length; index++)
    {
        AnimationEntity element = this->animationEntitys[index];
        element.open = false;
    }
    this->animationEntitys = new TArray<AnimationEntity>();

    foreach(var v in this->allNodes)
    {
        ResCenter.Instance.mCardAnimationItemPool.recycleObj(v);
    }
    this->allNodes.Clear();
    Destroy(gameObject);
}
    
void UAnimationView2_Default_Widget::onClick_Skip()
{
    this->skipNode.SetActive(false);
    AudioController.Instance.playSound(Sounds.button, 1);
    this->onAnimatinCallBack();
    this->DoDestroyAction();
    TAController.Instance.trackAnimationSkip();
}