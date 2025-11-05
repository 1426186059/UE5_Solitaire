//using System;
//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
//using UnityEngine.SocialPlatforms;
//using UnityEngine.UI;
//
//public class AnimationView8_butterfly : MonoBehaviour 
//{
//    // Card 数据
//    const float CardWidth = 103;
//    const float CardHeight = 154;
//    const float CardWidthSpaceX = 5;
//    public RectTransform ItemParent = null;
//    public RectTransform ItemParent2 = null;
//    public GameObject butterflyItemPrefab1;
//    public GameObject butterflyItemPrefab2;
//    public GameObject butterflyItemPrefab3;
//    // 52个 
//    TSArray<Vector3> aniPosArray = null;
//    TSArray<CardAnimationItem> animationEntitys = null;
//    TSMap<int, CardAnimationItem> animationEntityMap = null;
//    TSArray<CardType> colors = new TSArray<CardType>();
//    Action callBack = null;
//    public GameObject skipNode = null;
//    private AudioSource mBlastBgm = null;
//    private List<int> mTweenIdList = new List<int>();
//    
//    public void Show(AnimationType at, TSArray<CardType> colors, Vector3 startPt_w, float offsetX = 0, Action callback = null)
//    {
//        skipNode.SetActive(true);
//        this.gameObject.SetActive(true);
//        this.transform.localPosition = new Vector3(0, 0, 0);
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
//        return ((int)color - 1)*13 + value;
//    }
//
//    public void InitAni()
//    {
//        this.animationEntitys = new TSArray<CardAnimationItem>();
//        this.animationEntityMap = new TSMap<int, CardAnimationItem>();
//        Vector3 result1Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[0].transform.position, this.ItemParent);
//        for(int resultIndex = 3; resultIndex >= 0; resultIndex--) 
//        {
//            CardType mCardType = this.colors[resultIndex];  
//            float fXSpace1 = CardWidthSpaceX * resultIndex;
//            Vector3 frompt = new Vector3(result1Pos.x + resultIndex * (CardWidth-1) + fXSpace1, result1Pos.y, result1Pos.z);
//            for (int index = 13; index > 0; index--) 
//            {
//                CardAnimationItem mAniItem = ResCenter.readOnlyInstance.mCardAnimationItemPool.popObj();       
//                mAniItem.transform.SetParent(this.ItemParent);
//                mAniItem.transform.SetAsFirstSibling();
//                mAniItem.gameObject.SetActive(true);
//                
//                mAniItem.initByNum(index, mCardType);
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
//        this.DoAni1();
//    }
//    
//    private void DoAni1()
//    {
//        Vector3 result1Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[0].transform.position, this.ItemParent);
//        Vector3 result2Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[1].transform.position, this.ItemParent);
//        Vector3 result3Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[2].transform.position, this.ItemParent);
//        Vector3 result4Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[3].transform.position, this.ItemParent);
//        
//        int nAniIndex = 0;
//        for(int resultIndex = 0; resultIndex < 2; resultIndex++) 
//        {
//            for (int index = 13; index > 0; index--) 
//            {
//                int nIndex = resultIndex;
//                int nInde2 = index;
//                int nPokerId1 = this.getCardId(this.colors[resultIndex * 2], index);
//                CardAnimationItem mAniItem1 = this.animationEntityMap.get(nPokerId1);
//
//                int nPokerId2 = this.getCardId(this.colors[resultIndex * 2 + 1], index);
//                CardAnimationItem mAniItem2 = this.animationEntityMap.get(nPokerId2);
//                
//                int nCurAniIndex = nAniIndex++;
//                int id = LeanTween.delayedCall(gameObject, 1 + nCurAniIndex * 0.15f, ()=>
//                {
//                    mAniItem1.gameObject.SetActive(false);
//                    mAniItem2.gameObject.SetActive(false);
//                    GameObject goItem = Instantiate<GameObject>(butterflyItemPrefab3);
//                    goItem.transform.SetParent(ItemParent2, false);
//                    goItem.transform.SetAsFirstSibling();
//                    if (nIndex == 0)
//                    {
//                        goItem.transform.localPosition = (result1Pos + result2Pos) / 2f;
//                    }
//                    else
//                    {
//                        goItem.transform.localPosition = (result3Pos + result4Pos) / 2f;
//                    }
//                    goItem.GetComponent<AnimationView8_butterflyItem2>().Init(this);
//                    goItem.GetComponent<AnimationView8_butterflyItem2>().SetLeftRight(nPokerId1, nPokerId2);
//                    goItem.SetActive(true);
//                    goItem.GetComponent<AnimationView8_butterflyItem2>().FirstMove(nCurAniIndex, nIndex == 0);
//                }).id;
//                mTweenIdList.Add(id);
//            }
//        }
//
//        var id1 = LeanTween.delayedCall(6.0f, ()=>
//        {
//            this.onAnimationCallBack();
//        }).id;
//        mTweenIdList.Add(id1);
//    }
//
//    public void AddLeanTweenId(int id)
//    {
//        mTweenIdList.Add(id);
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
//            AudioController.Instance.StopSound(Sounds.blast_bgm);
//            foreach(CardAnimationItem v in this.animationEntitys)
//            {
//                LeanTween.cancel(v.gameObject);
//                ResCenter.Instance.mCardAnimationItemPool.recycleObj(v);
//            }
//
//            this.animationEntitys = null;
//            ItemParent2.gameObject.removeAllChildren();
//            Destroy(gameObject);
//        }
//    }
//    
//    public void onClick_Skip()
//    {
//        skipNode.SetActive(false);
//        AudioController.Instance.playSound(Sounds.button, 1);
//        this.onAnimationCallBack();
//        TAController.Instance.trackAnimationSkip();
//    }
//}