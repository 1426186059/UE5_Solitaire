#include "AnimationView2_Default_Widget.h"

void UAnimationView2_Default_Widget::Init()
{
    RectTransform tUITransform = GameLauncher.Instance.mUIRoot.mCanvas_Tip;
    this->animationSize = new Vector2(tUITransform.rect.width, tUITransform.rect.height);
    this->maxHeight = this->animationSize.y / 2;
    this->minHeight = -this->animationSize.y / 2 + 100;
    this->maxWidth = this->animationSize.x / 2 + CardWidth;
    this->minWidth = -this->animationSize.x / 2 - CardWidth;
    this->maxWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
    this->minWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
    this->cardsNode = this->transform.FindDeepChild("cardsnode").GetComponent<RectTransform>();
    this->cardsNode.gameObject.removeAllChildren();
}

void UAnimationView2_Default_Widget::Show(TFunction<void()> Show)
{
    this->animationOver = false;
    this->skipNode.SetActive(true);
    this->colors = new TArray<CardType>();
    for (int index = 0; index < colors.length; index++)
    {
        CardType element = colors[index];
        this->colors.push(element);
    }
    this->callBack = callback;
    this->showAnimation(colors, startPt_w, offsetX);
}

void UAnimationView2_Default_Widget::Show()
{
   
}

void UAnimationView2_Default_Widget::showAnimation(TArray<CardType> colors, FVector2D startPt_w, int offsetX = 0)
{
    FVector2D firstPt_l = GameTools.WorldToUILocalPos(startPt_w, this->cardsNode);
    this->showAnimation_Default(firstPt_l, colors, offsetX);
}

// 默认弹跳动画。
void UAnimationView2_Default_Widget::showAnimation_Default(FVector2D pt, TArray<CardType> colors, int offsetX)
{
    float delay = 0.1f;
    mBlastBgm = AudioController.Instance.playSound(Sounds.blast_bgm, 1);

    for (int index = 0; index < colors.length; index++)
    {
        CardType color = colors[index];
        int offset = offsetX * index;
        FVector2D frompt = new FVector2D(pt.x + index * (CardWidth - 1) + offset, pt.y, pt.z);
        delay = index * 0.5f; //+ 0.1;  
        this->showAnimation_Default_Col(index, frompt, delay, color, offsetX);
    }
}

// 每一个位置的动画
void UAnimationView2_Default_Widget::showAnimation_Default_Col(int colindex, FVector2D pt, float delay, CardType color, int offsetX = 0)
{
    float delayvalue = 0;
    float delayoffset = 0.1f;
    for (int index = 13; index > 0; index--)
    {
        this->showAnimation_Default_ColValue(colindex, pt, delay + delayvalue, color, index, offsetX);
        delayvalue += delayoffset;
    }
}

// 每一列中，单个数字的移动。
void UAnimationView2_Default_Widget::showAnimation_Default_ColValue(int colindex, FVector2D pt, float delay, CardType color, int value, int offsetX = 0)
{
    GameObject node ret = this->addStaticCard(pt, color, value, colindex);
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
UWidget* UAnimationView2_Default_Widget::CreateCard(FVector2D pt, int colorType, int value, int colindex)
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