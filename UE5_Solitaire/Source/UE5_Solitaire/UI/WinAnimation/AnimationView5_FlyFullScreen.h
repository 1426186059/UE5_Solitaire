//using System;
//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
//using UnityEngine.UI;
//
//public class AnimationView5_FlyFullScreen : MonoBehaviour
//{
//    class AnimationEntity
//    {
//        public int index = 0; //第几个col的
//        public int value = 13;
//        public int cardid = 1;  //13*v+value cardid
//
//        public CardType color = CardType.FangPian;
//        public bool open = false;
//        public bool trigger = false;
//        public float triggerDelay = 0;
//        public bool btoRight = true;
//        public float checktimes = 0; //每两帧，检查一次位置。
//
//        public Vector3 startPt = new Vector3(0, 0, 0);  //最开始的起始点。
//        public Vector3 nowPt = new Vector3(0, 0, 0);
//
//        public float vx = 0;    // x方向的速度
//        public float vy = 0;    // y方向的速度  
//        public float vyMax = 0; // y方向的最大速度
//        public float vx_a = 0;  // x方向的加速度 x轴匀速
//        public float vy_a = 0;  // y方向的加速度
//        public GameObject firstNode = null!;
//        public float maxHeight = 0;   //每次更新最高值。
//
//        public static bool toRight()
//        {
//            bool toRight = RandomTool.RandomInt(0, 100) < 50;
//            return toRight;
//            //return true;
//        }
//        
//        public static float randomVx()
//        {
//            return UnityEngine.Random.Range(150, 350);
//        }
//
//        public static float randomVy()
//        {
//            return UnityEngine.Random.Range(400, 600);
//        }
//
//        public static float randomVy_a()
//        {
//            return UnityEngine.Random.Range(8000, 10000) * (-1);
//        }
//
//        public static int getCardId(CardType color, int value)
//        {
//            return (int)color * 13 + value;
//        }
//
//        public static AnimationEntity create(int index, CardType color, int value)
//        {
//            AnimationEntity entity = new AnimationEntity();
//            entity.color = color;
//            entity.index = index;
//            entity.value = value;
//            entity.cardid = AnimationEntity.getCardId(color, value);
//            return entity;
//        }
//    }
//
//    const float CardWidth = 103;
//    const float CardHeigt = 154;
//    const float CardsColTotal = 120; //每个数字最多显示多少张card，避免过多导致卡顿。
//    const float Delay_Col_Offset = 0.1f; //5*2/30
//    const float Delay_Value_Offset = 0.01f; // 22*2/30
//
//    public GameObject skipNode = null;
//    Action callBack = null;
//    Vector2 animationSize = new Vector2(0, 0);
//
//    float maxHeight = 0;
//    float minHeight = 0;
//    float maxWidth = 0;
//    float minWidth = 0;
//
//    //所有的card，为了添加节点的时候。
//    TSArray<CardAnimationItem> allNodes = new TSArray<CardAnimationItem>();
//    // 52个 
//    TSArray<AnimationEntity> animationEntitys = new TSArray<AnimationEntity>();
//    bool animationOver = false;
//    RectTransform cardsNode = null;
//    TSArray<CardType> colors = new TSArray<CardType>();
//
//    void Awake()
//    {
//        RectTransform tUITransform = GameLauncher.Instance.mUIRoot.mCanvas_Game;
//        this.animationSize = new Vector2(tUITransform.rect.width, tUITransform.rect.height);
//        this.maxHeight = this.animationSize.y / 2;
//        this.minHeight = -this.animationSize.y / 2 + 100;
//        this.maxWidth = this.animationSize.x / 2;
//        this.minWidth = -this.animationSize.x / 2;
//
//        this.maxWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
//        this.minWidth /= GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.localScale.x;
//        
//        this.cardsNode = this.transform.FindDeepChild("cardsnode").GetComponent<RectTransform>();
//        this.cardsNode.gameObject.removeAllChildren();
//    }
//
//    public void Show(TSArray<CardType> colors, Vector3 startPt_w, int offsetX, Action callback)
//    {
//        this.animationOver = false;
//        this.skipNode.SetActive(true);
//        this.colors = new TSArray<CardType>();
//        for (int index = 0; index < colors.length; index++)
//        {
//            CardType element = colors[index];
//            this.colors.push(element);
//        }
//        this.callBack = callback;
//        this.showAnimation(colors, startPt_w, offsetX);
//
//        LeanTween.delayedCall(6, ()=>
//        {
//            this.onAnimatinCallBack();
//        });
//    }
//
//    void showAnimation(TSArray<CardType> colors, Vector3 startPt_w, int offsetX = 0)
//    {
//        Vector3 firstPt_l = GameTools.WorldToUILocalPos(startPt_w, this.cardsNode);
//        this.showAnimation_Default(firstPt_l, colors, offsetX);
//    }
//        
//    AudioSource mBlastBgm = null;
//    void showAnimation_Default(Vector3 pt, TSArray<CardType> colors, int offsetX)
//    {
//        mBlastBgm = AudioController.Instance.playSound(Sounds.blast_bgm, 1);
//        int nAniIndex = 0;
//        int nHengIndex = 0;
//        for (int i = colors.length - 1; i >= 0; i--)
//        {
//            CardType color = colors[i];
//            int offset = offsetX * i;
//            Vector3 frompt = new Vector3(pt.x + i * (CardWidth - 1) + offset, pt.y, pt.z);
//            int nShuIndex = 0;
//            for (int j = 13; j > 0; j--)
//            {
//               //float delay = nShuIndex * UnityEngine.Random.Range(1f, 6f) + nHengIndex * UnityEngine.Random.Range(0.1f, 0.5f);
//                float delay = nShuIndex * 0.7f + nHengIndex * 0.18f;
//                this.showAnimation_Default_ColValue(0, frompt, delay, color, j, offsetX);
//                nAniIndex++;
//                nShuIndex++;
//            }
//            nHengIndex++;
//        }
//    }
//    
//    // 每一列中，单个数字的移动。
//    void showAnimation_Default_ColValue(int colindex, Vector3 pt, float delay, CardType color, int value, int offsetX = 0)
//    {
//        CardAnimationItem ret = this.addStaticCard(pt, color, value);
//        var startNode = ret.gameObject;    
//
//        var entity = AnimationEntity.create(colindex, color, value);
//        entity.open = true;
//        entity.trigger = false;
//        entity.triggerDelay = delay;
//        entity.btoRight = AnimationEntity.toRight();
//        entity.startPt = pt;
//        entity.nowPt = pt;
//        entity.firstNode = startNode;
//        entity.maxHeight = this.maxHeight;
//        entity.vx = AnimationEntity.randomVx();
//        entity.vx_a = 0;
//
//        entity.vy = AnimationEntity.randomVy();
//        if (!entity.btoRight)
//        {
//            entity.vx *= -1;
//        }
//        
//        entity.vy_a = AnimationEntity.randomVy_a();
//        entity.firstNode = startNode;
//        this.animationEntitys.push(entity);
//        startNode.transform.SetSiblingIndex(0);
//    }
//
//    CardAnimationItem addStaticCard(Vector3 pt, CardType colorType, int value)
//    {
//        CardAnimationItem startNode = ResCenter.Instance.mCardAnimationItemPool.popObj();
//        startNode.transform.SetParent(this.cardsNode.transform, false);
//        startNode.transform.localPosition = pt;
//        startNode.initByNum(value, colorType);
//        this.allNodes.push(startNode);
//        startNode.gameObject.SetActive(true);
//        return startNode;
//    }
//
//    void updateAnimation(AnimationEntity entity, float dt)
//    {
//        if (!entity.open)
//        {
//            return;
//        }
//
//        if (entity.trigger)
//        {
//            if (entity.firstNode == null)
//            {
//                return;
//            }
//
//            var startPt = entity.nowPt;
//            var firstNode = entity.firstNode;
//            var maxHeight = entity.maxHeight;
//            var toRight = entity.btoRight;
//            var vx_a = entity.vx_a;
//            var vy_a = entity.vy_a;
//            var nowPt = new Vector3(0, 0, 0);
//
//            entity.vx += vx_a * dt;
//            entity.vy += vy_a * dt;
//            var vx = entity.vx;
//            var vy = entity.vy;
//
//            nowPt.x = (float)(startPt.x + vx * dt + 0.5f * vx_a * dt * dt);
//            nowPt.y = (float)(startPt.y + vy * dt + 0.5f * vy_a * dt * dt);
//            nowPt.z = startPt.z;
//
//            if (nowPt.y < this.minHeight)
//            {
//                nowPt.y = this.minHeight;
//                entity.vy *= -0.95f;  //转变方向
//            }
//
//            if (nowPt.y > entity.maxHeight)
//            {
//                nowPt.y = entity.maxHeight;
//                entity.vy = 0;
//                entity.maxHeight = entity.maxHeight * 0.7f;
//            }
//
//            entity.checktimes += 1;
//            firstNode.transform.localPosition = nowPt;
//            entity.nowPt = nowPt;
//            
//            CloneFlyObj(entity);
//            if (Mathf.Abs(nowPt.x) > this.maxWidth + 50)
//            {
//                entity.open = false;
//            }
//        }
//        else
//        {
//            entity.triggerDelay -= dt;
//            if (entity.triggerDelay <= 0)
//            {
//                entity.trigger = true;
//                entity.firstNode.gameObject.SetActive(false);
//            }
//        }
//    }
//
//    public void Update()
//    {
//        if(gameObject == null) return;
//
//        float dt = Time.deltaTime;
//        while(dt > 0)
//        {
//            dt -= 0.025f;
//            for (int index = 0; index < this.animationEntitys.length; index++)
//            {
//                AnimationEntity entity = this.animationEntitys[index];
//                this.updateAnimation(entity, 0.025f);
//            }
//        }
//    }
//    
//    TSMap<GameObject, Timer> mTimerDic = new TSMap<GameObject, Timer>();
//    private void CloneFlyObj(AnimationEntity mEntry)
//    {
//       CardAnimationItem mObj = addStaticCard(mEntry.firstNode.transform.localPosition, mEntry.color, mEntry.value);
//       mObj.gameObject.SetActive(true);
//       
//       var mTimer = Timer.New(()=>
//       {
//            this.allNodes.Remove(mObj);
//            ResCenter.Instance.mCardAnimationItemPool.recycleObj(mObj);
//       }, 3.0f);
//       mTimer.Start();
//       mTimerDic.set(mObj.gameObject, mTimer);
//    }
//
//    void onAnimatinCallBack()
//    {
//        if (this.callBack != null)
//        {
//            AudioController.Instance.StopSound(Sounds.blast_bgm);
//            this.callBack();
//            this.callBack = null;
//            for (int index = 0; index < this.animationEntitys.length; index++)
//            {
//                AnimationEntity element = this.animationEntitys[index];
//                element.open = false;
//            }
//
//            foreach(var v in mTimerDic)
//            {
//                v.Value.Stop();
//            }
//            mTimerDic.Clear();
//        }
//    }
//
//    public void DoDestroyAction()
//    {      
//        if (this.animationOver)
//        {
//            return;
//        }
//        
//        this.animationOver = true;
//        for (int index = 0; index < this.animationEntitys.length; index++)
//        {
//            AnimationEntity element = this.animationEntitys[index];
//            element.open = false;
//        }
//        this.animationEntitys = new TSArray<AnimationEntity>();
//
//        foreach(var v in this.allNodes)
//        {
//            ResCenter.Instance.mCardAnimationItemPool.recycleObj(v);
//        }
//        this.allNodes.Clear();
//        Destroy(gameObject);
//    }
//    
//    public void onClick_Skip()
//    {
//        skipNode.SetActive(false);
//        AudioController.Instance.playSound(Sounds.button, 1);
//        this.onAnimatinCallBack();
//        TAController.Instance.trackAnimationSkip();
//    }
//
//
//}