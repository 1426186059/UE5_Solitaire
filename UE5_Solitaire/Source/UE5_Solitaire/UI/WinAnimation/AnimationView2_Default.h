using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AnimationView2_Default : MonoBehaviour
{
    class AnimationEntity
    {
        public int index = 0; //第几个col的
        public int value = 13;
        public int cardid = 1;  //13*v+value cardid

        public CardType color = CardType.FangPian;
        public bool open = false;
        public bool trigger = false;
        public float triggerDelay = 0;
        public bool btoRight = true;
        public float deltTime = 0;
        public float checktimes = 0; //每两帧，检查一次位置。

        public Vector3 startPt = new Vector3(0, 0, 0);  //最开始的起始点。
        public Vector3 nowPt = new Vector3(0, 0, 0);

        public float vx = 0;    // x方向的速度
        public float vy = 0;    // y方向的速度  
        public float vyMax = 0; // y方向的最大速度
        public float vx_a = 0;  // x方向的加速度 x轴匀速
        public float vy_a = 0;  // y方向的加速度
        public GameObject firstNode = null!;
        public float maxHeight = 0;   //每次更新最高值。

        public static bool toRight(int index)
        {
            int[] toRightRate = new int[] { 100, 50, 40, 30 };
            int rate = UnityEngine.Random.Range(0, 100);
            if(DataCenter.Instance.sysConfig.leftHand == 1)
            {
                rate = UnityEngine.Random.Range(0, 200);
            }
            bool toRight = rate <= toRightRate[index];
            return toRight;
        }

        public static float randomVx()
        {
            return UnityEngine.Random.Range(200, 260);
        }

        public static float randomVy()
        {
            return UnityEngine.Random.Range(500, 600);
        }

        public static float randomVy_a()
        {
            return UnityEngine.Random.Range(5500, 6200) * (-1);
        }

        public static int getCardId(CardType color, int value)
        {
            return (int)color * 13 + value;
        }

        public static AnimationEntity create(int index, CardType color, int value)
        {
            AnimationEntity entity = new AnimationEntity();
            entity.color = color;
            entity.index = index;
            entity.value = value;
            entity.cardid = AnimationEntity.getCardId(color, value);
            return entity;
        }
    }
        
    // Card 数据
    const float CardWidth = 103;
    const float CardHeigt = 154;
    const float CardsColTotal = 120; //每个数字最多显示多少张card，避免过多导致卡顿。

    const float Delay_Col_Offset = 0.1f; //5*2/30
    const float Delay_Value_Offset = 0.01f; // 22*2/30

    public GameObject skipNode = null!;
    Action callBack = null;
    Vector2 animationSize = new Vector2(0, 0);

    float maxHeight = 0;
    float minHeight = 0;
    float maxWidth = 0;
    float minWidth = 0;

    // 默认动画 是否触发

    // 记录52个部分 .记录每一个card出了多少张。
    TSMap<int, TSArray<CardAnimationItem>> colNodes_Dic = new TSMap<int, TSArray<CardAnimationItem>>();
    //所有的card，为了添加节点的时候。
    TSArray<CardAnimationItem> allNodes = new TSArray<CardAnimationItem>();

    // 52个 
    TSArray<AnimationEntity> animationEntitys = new TSArray<AnimationEntity>();
    bool animationOver = false;
    RectTransform cardsNode = null;
    TSArray<CardType> colors = new TSArray<CardType>();
    
    void Awake()
    {
        RectTransform tUITransform = GameLauncher.Instance.mUIRoot.mCanvas_Tip;
        this.animationSize = new Vector2(tUITransform.rect.width, tUITransform.rect.height);

        this.maxHeight = this.animationSize.y / 2;
        this.minHeight = -this.animationSize.y / 2 + 100;
        this.maxWidth = this.animationSize.x / 2 + CardWidth;
        this.minWidth = -this.animationSize.x / 2 - CardWidth;

        this.maxWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
        this.minWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
        
        this.cardsNode = this.transform.FindDeepChild("cardsnode").GetComponent<RectTransform>();
        this.cardsNode.gameObject.removeAllChildren();
    }

    public void Show(TSArray<CardType> colors, Vector3 startPt_w, int offsetX, Action callback)
    {
        this.animationOver = false;
        this.skipNode.SetActive(true);
        this.colors = new TSArray<CardType>();
        for (int index = 0; index < colors.length; index++)
        {
            CardType element = colors[index];
            this.colors.push(element);
        }
        this.callBack = callback;
        this.showAnimation(colors, startPt_w, offsetX);
    }

    void showAnimation(TSArray<CardType> colors, Vector3 startPt_w, int offsetX = 0)
    {
        Vector3 firstPt_l = GameTools.WorldToUILocalPos(startPt_w, this.cardsNode);
        this.showAnimation_Default(firstPt_l, colors, offsetX);
    }

    AudioSource mBlastBgm = null;
    // 默认弹跳动画。
    void showAnimation_Default(Vector3 pt, TSArray<CardType> colors, int offsetX)
    {
        float delay = 0.1f;
        mBlastBgm = AudioController.Instance.playSound(Sounds.blast_bgm, 1);
        
        for (int index = 0; index < colors.length; index++)
        {
            CardType color = colors[index];
            int offset = offsetX * index;
            Vector3 frompt = new Vector3(pt.x + index * (CardWidth - 1) + offset, pt.y, pt.z);
            delay = index * 0.5f; //+ 0.1;  
            this.showAnimation_Default_Col(index, frompt, delay, color, offsetX);
        }
    }

    // 每一个位置的动画
    void showAnimation_Default_Col(int colindex, Vector3 pt, float delay, CardType color, int offsetX = 0)
    {
        float delayvalue = 0;
        float delayoffset = 0.1f;
        for (int index = 13; index > 0; index--)
        {
            this.showAnimation_Default_ColValue(colindex, pt, delay + delayvalue, color, index, offsetX);
            delayvalue += delayoffset;
        }
    }


    // 每一列中，单个数字的移动。
    void showAnimation_Default_ColValue(int colindex, Vector3 pt, float delay, CardType color, int value, int offsetX = 0)
    {
        (GameObject node, bool) ret = this.addStaticCard(pt, color, value, colindex);
        var startNode = ret.node;    

        var entity = AnimationEntity.create(colindex, color, value);
        entity.open = true;
        entity.trigger = false;
        entity.triggerDelay = delay;
        entity.btoRight = AnimationEntity.toRight(colindex);
        entity.startPt = pt;
        entity.nowPt = pt;
        entity.firstNode = startNode;
        entity.maxHeight = this.maxHeight;

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
        this.animationEntitys.push(entity);
        startNode.transform.SetSiblingIndex(0);

    }

    // 检查是否最后一个队列中的最后一个，标志动画结束。   
    (GameObject, bool) addStaticCard(Vector3 pt, CardType colorType, int value, int colindex)
    {
        int nodekey = AnimationEntity.getCardId(colorType, value);
        TSArray<CardAnimationItem> nodeArrs = this.colNodes_Dic.get(nodekey);
        if (nodeArrs == null)
        {
            nodeArrs = new TSArray<CardAnimationItem>();
            this.colNodes_Dic.set(nodekey, nodeArrs);
        }

        bool firstNodeByNewValue = false;
        if (value == 13 && nodeArrs.length == 0)
        {
            firstNodeByNewValue = true;
        }

        AnimationEntity entity = null;
        // 从后往前找，找到第一个。
        for (int index = this.animationEntitys.length - 1; index >= 0; index--)
        {
            AnimationEntity element = this.animationEntitys[index];
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
                if (entity.color == this.colors[this.colors.length - 1] && entity.value == 1)
                {
                    this.onAnimatinCallBack();
                    this.DoDestroyAction();
                }

            }
            return (null, firstNodeByNewValue);
        }
        
        CardAnimationItem startNode = ResCenter.Instance.mCardAnimationItemPool.popObj();
        startNode.transform.SetParent(this.cardsNode.transform, false);
        startNode.transform.localPosition = pt;
        nodeArrs.push(startNode);

        startNode.initByNum(value, colorType);

        this.allNodes.push(startNode);
        return (startNode.gameObject, firstNodeByNewValue);
    }


    void updateAnimation(AnimationEntity entity, float dt)
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

            var nowPt = new Vector3(0, 0, 0);

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
            if (nowPt.y < this.minHeight)
            {
                nowPt.y = this.minHeight;
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
            // 水平 不会碰撞后返回了。
            if (nowPt.x < this.minWidth - CardWidth)
            {
                nowPt.x = this.minWidth;
                willRemove = true;
            }

            if (nowPt.x > this.maxWidth + CardWidth)
            {
                nowPt.x = this.maxWidth;
                entity.vx *= -1;
                willRemove = true;


            }

            // 每两帧之间 添加
            entity.checktimes += 1;

            firstNode.transform.localPosition = nowPt;
            entity.nowPt = nowPt;

            // 碰到边界，不在产生新的额
            if (entity.open)
            {
                firstNode.transform.localPosition = nowPt;
                if (willRemove)
                {
                    entity.open = false;
                    if (entity.value == 6 && entity.index == 2)
                    {

                        this.onAnimatinCallBack();
                        this.DoDestroyAction();
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
    
    public void Update()
    {
        if(gameObject == null) return;

        float dt = Time.deltaTime;
        for (int index = 0; index < this.animationEntitys.length; index++)
        {
            AnimationEntity entity = this.animationEntitys[index];
            this.updateAnimation(entity, dt);
        }
    }

    void onAnimatinCallBack()
    {
        if (this.callBack != null)
        {
            AudioController.Instance.StopSound(Sounds.blast_bgm);
            this.callBack();
            this.callBack = null;
        }
    }

    public void DoDestroyAction()
    {      
        if (this.animationOver)
        {
            return;
        }

        this.animationOver = true;
        for (int index = 0; index < this.animationEntitys.length; index++)
        {
            AnimationEntity element = this.animationEntitys[index];
            element.open = false;
        }
        this.animationEntitys = new TSArray<AnimationEntity>();
        
        foreach(var v in this.allNodes)
        {
            ResCenter.Instance.mCardAnimationItemPool.recycleObj(v);
        }
        this.allNodes.Clear();
        Destroy(gameObject);
    }   
    
    public void onClick_Skip()
    {
        this.skipNode.SetActive(false);
        AudioController.Instance.playSound(Sounds.button, 1);
        this.onAnimatinCallBack();
        this.DoDestroyAction();
        TAController.Instance.trackAnimationSkip();
    }


}