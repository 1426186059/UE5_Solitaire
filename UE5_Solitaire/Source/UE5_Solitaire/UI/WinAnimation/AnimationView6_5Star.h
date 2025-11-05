//using System;
//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
//using UnityEngine.Playables;
//using UnityEngine.SocialPlatforms;
//using UnityEngine.UI;
//
//public class AnimationView6_5Star : MonoBehaviour 
//{
//    // Card 数据
//    const float CardWidth = 103;
//    const float CardHeight = 154;
//    const float CardWidthSpaceX = 5;
//    public RectTransform ItemParent = null;
//    
//    // 52个 
//    TSArray<Vector3> aniPosArray = null;
//    TSArray<CardAnimationItem> animationEntitys = null;
//    TSMap<int, CardAnimationItem> animationEntityMap = null;
//    TSArray<CardType> colors = new TSArray<CardType>();
//    Action callBack = null;
//
//    private AudioSource mBlastBgm = null;
//    private List<int> mTweenIdList = new List<int>();
//    private bool bCollectPosFinish = false;
//
//    public GameObject skipNode = null;
//
//    public void Show(AnimationType at, TSArray<CardType> colors, Vector3 startPt_w, float offsetX = 0, Action callback = null)
//    {
//        skipNode.SetActive(true);
//        this.gameObject.SetActive(true);
//        this.transform.localPosition = Vector3.zero;
//        mBlastBgm = AudioController.Instance.playSound(Sounds.blast_bgm, 1);
//    
//        this.colors = new TSArray<CardType>();
//        for (int index = 0; index < colors.length; index++) 
//        {
//            var element = colors[index];
//            this.colors.push(element);
//        }
//        this.callBack = callback;
//        this.InitAni();
//    }
//
//    int getCardId(CardType color, int value)
//    {
//        return ((int)color - 1) * 13 + value;
//    }
//
//    public void InitAni()
//    {
//        this.animationEntitys = new TSArray<CardAnimationItem>();
//        this.animationEntityMap = new TSMap<int, CardAnimationItem>();
//        Vector3 result1Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[0].transform.position, this.ItemParent);
//        for(int resultIndex = 0; resultIndex < 4; resultIndex++) 
//        {
//            CardType mCardType = this.colors[resultIndex];  
//            float fXSpace1 = CardWidthSpaceX * resultIndex;
//            Vector3 frompt = new Vector3(result1Pos.x + resultIndex * (CardWidth-1) + fXSpace1, result1Pos.y, result1Pos.z);
//            for (int index = 13; index > 0; index--) 
//            {
//                var mAniItem = ResCenter.readOnlyInstance.mCardAnimationItemPool.popObj();       
//                mAniItem.transform.SetParent(this.ItemParent);
//                mAniItem.transform.SetAsFirstSibling();
//                mAniItem.gameObject.SetActive(true);
//                mAniItem.initByNum(index, mCardType);
//
//                this.animationEntitys.push(mAniItem);
//                int nPokerId = mAniItem.nCardId;
//                this.animationEntityMap.set(nPokerId, mAniItem);
//                
//                mAniItem.transform.rotation = Quaternion.identity;
//                mAniItem.transform.localScale = new Vector3(1, 1, 1);
//                mAniItem.transform.localPosition = frompt;
//                mAniItem.n_root.transform.rotation = Quaternion.identity;
//                mAniItem.n_root.transform.localScale = new Vector3(1, 1, 1);
//                mAniItem.n_root.transform.localPosition = new Vector3(0, 0, 0);  
//            }
//        }
//
//        CollectAniPos();
//        this.DoAni1();
//    }
//
//    private void DoAni1()
//    {
//        int nAniIndex = 0;
//        for(int resultIndex = 0; resultIndex < 4; resultIndex++) 
//        {
//            for (int index = 13; index > 0; index--) 
//            {
//                int nPokerId = this.getCardId(this.colors[resultIndex], index);
//                CardAnimationItem mAniItem = this.animationEntityMap.get(nPokerId);
//                int nCurAniIndex = nAniIndex++;
//
//                int nTargetIndex = nCurAniIndex % aniPosArray.Count;
//                Vector3 targetPos = this.aniPosArray[nTargetIndex];
//                int id = LeanTween.delayedCall(gameObject, 1 + nCurAniIndex * 0.04f, ()=>
//                {
//                    mAniItem.transform.SetAsLastSibling();
//                    int id = LeanTween.moveLocal(mAniItem.gameObject, targetPos, 0.5f).setOnComplete(()=>
//                    {
//                        this.DoAni2(mAniItem, nTargetIndex + 1);
//                    }).id;
//                    mTweenIdList.Add(id);
//                }).id;
//                mTweenIdList.Add(id);
//            }
//        }
//        
//        var id1 = LeanTween.delayedCall(4f, ()=>
//        {
//            onAnimationCallBack();
//        }).id;
//        mTweenIdList.Add(id1);
//    }
//
//    private void DoAni2(CardAnimationItem mAniItem, int nTargetIndex)
//    {
//        nTargetIndex = nTargetIndex % aniPosArray.length;
//        Vector3 targetPos = this.aniPosArray[nTargetIndex];
//        var id = LeanTween.moveLocal(mAniItem.gameObject, targetPos, 1f).setOnComplete(()=>
//        {
//            this.DoAni2(mAniItem, nTargetIndex + 1);
//        }).id;
//        mTweenIdList.Add(id);
//    }
//    
//    void CollectAniPos()
//    {
//        aniPosArray = new TSArray<Vector3>();
//        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos1").position, ItemParent));
//        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos2").position, ItemParent));
//        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos3").position, ItemParent));
//        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos4").position, ItemParent));
//        aniPosArray.Add(GameTools.WorldToUILocalPos(transform.FindDeepChild("AniPos/Pos5").position, ItemParent));
//    }
//
//    private void PrintCollectPos(Vector3 pos)
//    {
//        CardAnimationItem startNode = ResCenter.readOnlyInstance.mCardAnimationItemPool.popObj();       
//        startNode.transform.SetParent(this.ItemParent, false);
//        startNode.transform.SetAsFirstSibling();
//        startNode.transform.localPosition = pos;
//        startNode.gameObject.SetActive(true);
//    }
//
//    private void CancelLeanTween()
//    {
//        foreach(var v in mTweenIdList)
//        {
//            if(LeanTween.isTweening(v))
//            {
//                LeanTween.cancel(v);
//            }
//        }
//        mTweenIdList.Clear();
//    }
//
//    public void onClick_Skip()
//    {
//        skipNode.SetActive(false);
//        AudioController.Instance.playSound(Sounds.button, 1);
//        this.onAnimationCallBack();
//        TAController.Instance.trackAnimationSkip();
//    }
//        
//    private void onAnimationCallBack()
//    {               
//        if(this.callBack != null)
//        {
//            this.callBack();
//            this.callBack = null;
//
//            AudioController.Instance.StopSound(Sounds.blast_bgm);
//            CancelLeanTween();
//        }
//    }
//
//    public void DoDestroyAction()
//    {   
//        if(this.gameObject != null)
//        {
//            CancelLeanTween();
//            AudioController.Instance.StopSound(Sounds.blast_bgm);
//            foreach(CardAnimationItem v in this.animationEntitys)
//            {
//                LeanTween.cancel(v.gameObject);
//                ResCenter.Instance.mCardAnimationItemPool.recycleObj(v);
//            }
//
//            this.animationEntitys = null;
//            Destroy(gameObject);
//        }
//    }
//}