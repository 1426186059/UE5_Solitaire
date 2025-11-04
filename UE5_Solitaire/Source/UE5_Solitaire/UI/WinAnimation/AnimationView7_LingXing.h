using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SocialPlatforms;
using UnityEngine.UI;

public class AnimationView7_LingXing : MonoBehaviour 
{
    // Card 数据
    const float CardWidth = 103;
    const float CardHeight = 154;
    const float CardWidthSpaceX = 5;
    public RectTransform ItemParent = null;
    Transform n_targetPos1 = null;
    public GameObject goWinAnimation_Ani;
    public GameObject skipNode = null;
    // 52个 
    TSArray<Vector3> aniPosArray = null;
    TSArray<Vector3> aniPosArray1 = null;
    TSArray<Vector3> aniPosArray2 = null;
    TSArray<CardAnimationItem> animationEntitys = null;
    TSMap<int, CardAnimationItem> animationEntityMap = null;
    TSArray<CardType> colors = new TSArray<CardType>();
    Action callBack = null;

    private AudioSource mBlastBgm = null;
    private List<int> mTweenIdList = new List<int>();
    private bool bCollectPosFinish = false;
    
    public void Show(AnimationType at, TSArray<CardType> colors, Vector3 startPt_w, float offsetX = 0, Action callback = null)
    {
        this.skipNode.SetActive(true);
        this.gameObject.SetActive(true);
        this.transform.localPosition = new Vector3(0, 0, 0);
        mBlastBgm = AudioController.Instance.playSound(Sounds.blast_bgm, 1);

        this.colors = new TSArray<CardType>();
        for (int index = 0; index < colors.length; index++) 
        {
            var element = colors[index];
            this.colors.push(element);
        }
        this.callBack = callback;

        this.CollectAniPos();
        this.InitAni();
    }

    int getCardId(CardType color, int value)
    {
        return ((int)color - 1)*13 + value;
    }

    public void InitAni()
    {
        this.animationEntitys = new TSArray<CardAnimationItem>();
        this.animationEntityMap = new TSMap<int, CardAnimationItem>();
        Vector3 result1Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[0].transform.position, this.ItemParent);
        for(int resultIndex = 3; resultIndex >= 0; resultIndex--) 
        {
            CardType mCardType = this.colors[resultIndex];  
            float fXSpace1 = CardWidthSpaceX * resultIndex;
            Vector3 frompt = new Vector3(result1Pos.x + resultIndex * (CardWidth-1) + fXSpace1, result1Pos.y, result1Pos.z);
            for (int index = 13; index > 0; index--) 
            {
                CardAnimationItem mAniItem = ResCenter.readOnlyInstance.mCardAnimationItemPool.popObj();       
                mAniItem.transform.SetParent(this.ItemParent);
                mAniItem.transform.SetAsFirstSibling();
                mAniItem.gameObject.SetActive(true);
                
                mAniItem.initByNum(index, mCardType);
                this.animationEntitys.push(mAniItem);
                int nPokerId = mAniItem.nCardId;
                this.animationEntityMap.set(nPokerId, mAniItem);
                
                mAniItem.transform.rotation = Quaternion.identity;
                mAniItem.transform.localScale = new Vector3(1, 1, 1);
                mAniItem.transform.localPosition = frompt;
                mAniItem.n_root.transform.rotation = Quaternion.identity;
                mAniItem.n_root.transform.localScale = new Vector3(1, 1, 1);
                mAniItem.n_root.transform.localPosition = new Vector3(0, 0, 0);
            }
        }
        this.DoAni1();
    }

    private void DoAni1()
    {
        int nAniIndex = 0;
        for(int resultIndex = 0; resultIndex < 4; resultIndex++) 
        {
            for (int index = 13; index > 0; index--) 
            {
                int nPokerId = this.getCardId(this.colors[resultIndex], index);
                CardAnimationItem mAniItem = this.animationEntityMap.get(nPokerId);
                Vector3 targetPos = new Vector3(0, 0, 0);
                int nCurAniIndex = ++nAniIndex;

                int id = LeanTween.delayedCall(gameObject, 1 + (nCurAniIndex - 1) * 0.03f, ()=>
                {
                    Vector3 beginPos = mAniItem.transform.localPosition;
                    Vector3 endPos = this.aniPosArray1[nCurAniIndex + 20];
                    
                    Vector3[] mPosArray = new Vector3[4];
                    mPosArray[0] = beginPos;
                    mPosArray[1] = Vector3.zero;
                    mPosArray[2] = Vector3.zero;
                    mPosArray[3] = endPos;

                    mAniItem.transform.SetAsLastSibling();
                    int id = LeanTween.moveLocal(mAniItem.gameObject, mPosArray, 0.4f).setOnComplete(()=>
                    {
                        var mItem = new AniItem();
                        mItem.nPosIndex = this.FindBestTargetIndex(mAniItem.transform.localPosition);
                        mItem.obj = mAniItem.gameObject;
                        mItemList.Add(mItem);
                    }).id;
                    mTweenIdList.Add(id);
                }).id;
                mTweenIdList.Add(id);
            }
        }

        var id1 = LeanTween.delayedCall(3.5f, ()=>
        {
            this.onAnimationCallBack();
        }).id;
        mTweenIdList.Add(id1);
    }

    public int FindBestTargetIndex(Vector3 pos)
    {
        int nTargetIndex = -1;
        float fMinDistance = float.MaxValue;
        for(int i = 0; i < aniPosArray2.length; i++)
        {
            var fDistance = Vector3.Distance(aniPosArray2[i], pos);
            if(fDistance < fMinDistance)
            {
                nTargetIndex = i;
                fMinDistance = fDistance;
            }
        }
        return nTargetIndex;
    }

    public class AniItem
    {
        public GameObject obj;
        public int nPosIndex;
    }

    private readonly List<AniItem> mItemList = new List<AniItem>();
    float fAniInternalTime = 0f;
    private void Update()
    {
        float dt = Time.deltaTime;
        this.fAniInternalTime = this.fAniInternalTime + dt;
        float fTime = 0.03f;
        while (this.fAniInternalTime > fTime)
        {
            this.fAniInternalTime = this.fAniInternalTime - fTime;
            for (int index = 0; index < this.mItemList.Count; index++)
            {
                var go = mItemList[index].obj;
                int nPosIndex = mItemList[index].nPosIndex++;
                nPosIndex = nPosIndex % this.aniPosArray2.length;
                go.transform.localPosition = this.aniPosArray2[nPosIndex];
            }
        }
    }

    void CollectAniPos()
    {
        mItemList.Clear();

        aniPosArray = new TSArray<Vector3>();
        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos1").position, ItemParent));
        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos2").position, ItemParent));
        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos3").position, ItemParent));
        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos4").position, ItemParent));

        Vector3 mPos0 = aniPosArray[0];
        Vector3 mPos1 = aniPosArray[1];
        Vector3 mPos2 = aniPosArray[2];
        Vector3 mPos3 = aniPosArray[3];

        aniPosArray1 = new TSArray<Vector3>();
        for (int i = 0; i < 50; i++)
        {
            float fPercent = i / 49f;
            Vector3 mPos = mPos0 * (1 - fPercent) + mPos3 * fPercent;
            aniPosArray1.push(mPos);
        }

        for (int i = 0; i < 50; i++)
        {
            float fPercent = i / 49f;
            Vector3 mPos = mPos3 * (1 - fPercent) + mPos2 * fPercent;
            aniPosArray1.push(mPos);
        }

        aniPosArray2 = new TSArray<Vector3>();
        for (int i = 0; i < 13; i++)
        {
            float fPercent = i / 12f;
            Vector3 mPos = mPos0 * (1 - fPercent) + mPos1 * fPercent;
            aniPosArray2.push(mPos);
        }
        
        for (int i = 0; i < 13; i++)
        {
            float fPercent = i / 12f;
            Vector3 mPos = mPos1 * (1 - fPercent) + mPos2 * fPercent;
            aniPosArray2.push(mPos);
        }

        for (int i = 0; i < 13; i++)
        {
            float fPercent = i / 12f;
            Vector3 mPos = mPos2 * (1 - fPercent) + mPos3 * fPercent;
            aniPosArray2.push(mPos);
        }

        for (int i = 0; i < 13; i++)
        {
            float fPercent = i / 12f;
            Vector3 mPos = mPos3 * (1 - fPercent) + mPos0 * fPercent;
            aniPosArray2.push(mPos);
        }
    }

    private void Update_ForCollectPos()
    {
        Vector3 nPos = GameTools.WorldToUILocalPos(goWinAnimation_Ani.transform.position, this.ItemParent);
        this.aniPosArray.push(nPos);
        //PrintCollectPos(nPos);
    }
    
    public void OnAnimationEvent(object data) 
    {
        string aniEventName = (string)data;
        if(aniEventName == goWinAnimation_Ani.name)
        {
            onFire.Instance.off(GameEvents.OnAnimationEvent, this.OnAnimationEvent);
            TimeMgr.Instance.RemoveListener(Update_ForCollectPos);
            bCollectPosFinish = true;
            gameObject.SetActive(false);
            PrintTool.Log("this.aniPosArray.length: ", this.aniPosArray.length);
        }
        else
        {
            PrintTool.Log("OnAnimationEvent eventName: ", aniEventName);
        }
    }

    private void PrintCollectPos(Vector3 pos)
    {
        CardAnimationItem startNode = ResCenter.readOnlyInstance.mCardAnimationItemPool.popObj();       
        startNode.transform.SetParent(this.ItemParent, false);
        startNode.transform.SetAsFirstSibling();
        startNode.transform.localPosition = pos;
        startNode.gameObject.SetActive(true);
    }

    private void CancelLeanTween()
    {
        foreach(var v in mTweenIdList)
        {
            if(LeanTween.isTweening(v))
            {
                LeanTween.cancel(v);
            }
        }
        mTweenIdList.Clear();
    }

    private void onAnimationCallBack()
    {               
        if(this.callBack != null)
        {
            this.callBack();
            this.callBack = null;

            AudioController.Instance.StopSound(Sounds.blast_bgm);
            CancelLeanTween();
            mItemList.Clear();
        }
    }   
    
    public void DoDestroyAction()
    {   
        if(this.gameObject != null)
        {
            AudioController.Instance.StopSound(Sounds.blast_bgm);
            foreach(CardAnimationItem v in this.animationEntitys)
            {
                LeanTween.cancel(v.gameObject);
                ResCenter.Instance.mCardAnimationItemPool.recycleObj(v);
            }

            this.animationEntitys = null;
            onFire.Instance.off(GameEvents.OnAnimationEvent, this.OnAnimationEvent);
            mItemList.Clear();
            Destroy(gameObject);
        }
    }

    public void onClick_Skip()
    {
        this.skipNode.SetActive(false);
        AudioController.Instance.playSound(Sounds.button, 1);
        this.onAnimationCallBack();
        TAController.Instance.trackAnimationSkip();
    }
}