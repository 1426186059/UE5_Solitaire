//using System;
//using System.Collections;
//using System.Collections.Generic;
//using UnityEngine;
//using UnityEngine.UI;
//
//public class AnimationView9_smileface : MonoBehaviour 
//{
//    // Card 数据
//    const float CardWidth = 103;
//    const float CardHeight = 154;
//    const float CardWidthSpaceX = 5;
//    public RectTransform ItemParent = null;
//    public RectTransform SimpleFaceParent = null;
//    public GameObject ItemPrefab = null;
//
//    // 52个 
//    TSArray<Vector3> aniPosArray = null;
//    TSArray<CardAnimationItem> animationEntitys = null;
//    TSMap<int, CardAnimationItem> animationEntityMap = null;
//    TSArray<CardType> colors = new TSArray<CardType>();
//    Action callBack = null;
//    private AudioSource mBlastBgm = null;
//    private List<int> mTweenIdList = new List<int>();
//    public GameObject skipNode = null;
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
//        return ((int)color - 1) * 13 + value;
//    }
//
//    public void InitAni()
//    {
//        mItemList.Clear();
//        this.animationEntitys = new TSArray<CardAnimationItem>();
//        this.animationEntityMap = new TSMap<int, CardAnimationItem>();
//        Vector3 result1Pos = GameTools.WorldToUILocalPos(LobbyController.readOnlyInstance.n_card_results[0].transform.position, this.ItemParent);
//        for(int resultIndex = 3; resultIndex >= 0; resultIndex--) 
//        {
//            CardType mCardType = this.colors[resultIndex];  
//            float fXSpace1 = CardWidthSpaceX * resultIndex;
//            Vector3 fromPos = new Vector3(result1Pos.x + resultIndex * (CardWidth-1) + fXSpace1, result1Pos.y, result1Pos.z);
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
//                mAniItem.transform.localPosition = fromPos;
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
//        mItemList = new List<AnimationView9_smilefaceItem>();
//        for (int index = 13; index > 0; index--) 
//        {
//            for(int resultIndex = 0; resultIndex < 4; resultIndex++) 
//            {
//                int nIndex = resultIndex;
//                int nPokerId = this.getCardId(this.colors[resultIndex], index);
//                CardAnimationItem mAniItem = this.animationEntityMap.get(nPokerId);
//                mAniItem.gameObject.SetActive(false);
//
//                int nCurAniIndex = nAniIndex++;
//                if(nCurAniIndex < this.SimpleFaceParent.childCount)
//                {
//                    var go = Instantiate<GameObject>(ItemPrefab);
//                    go.transform.SetParent(ItemParent, false);
//                    go.transform.localPosition = mAniItem.transform.localPosition;
//                    go.SetActive(true);
//                    AnimationView9_smilefaceItem mAniItem1 = go.GetComponent<AnimationView9_smilefaceItem>();
//                    mAniItem1.SetLeftRight(nPokerId);
//                    mItemList.Add(mAniItem1);
//
//                    GameObject targetFaceObj = this.SimpleFaceParent.GetChild(nCurAniIndex).gameObject;
//                    int id = LeanTween.delayedCall(gameObject, 1 + nCurAniIndex * 0.05f, ()=>
//                    {
//                        mAniItem1.transform.SetAsLastSibling();
//                        Vector3 targetPos = targetFaceObj.transform.localPosition;
//                        Vector3 targetRotateAngle = targetFaceObj.transform.localEulerAngles;
//                        Vector3 targetScale = targetFaceObj.transform.localScale;
//                        LeanTween.rotateLocal(mAniItem1.goCenter, targetRotateAngle, 0.5f);
//                        LeanTween.moveLocal(mAniItem1.transform.gameObject, targetPos, 0.5f);
//                        LeanTween.scale(mAniItem1.transform.gameObject, targetScale, 0.5f);
//                    }).id;
//                    mTweenIdList.Add(id);
//                }
//            }
//        }
//
//        LeanTween.delayedCall(gameObject, 3.5f, ()=>
//        {
//            DoAni2();
//        });
//    }
//
//    List<AnimationView9_smilefaceItem> mItemList = new List<AnimationView9_smilefaceItem>();
//    public void DoAni2()
//    {
//        if(mItemList.Count == 0) return;
//        
//        List<List<int>> mAniList = new List<List<int>>();
//        mAniList.Add(new List<int>(){24, 25});
//        mAniList.Add(new List<int>(){23, 26});
//        mAniList.Add(new List<int>(){22, 27});
//        mAniList.Add(new List<int>(){21, 28, 1, 2});
//        mAniList.Add(new List<int>(){20, 29, 3, 11});
//        mAniList.Add(new List<int>(){19, 30, 4, 10});
//        mAniList.Add(new List<int>(){18, 31, 5, 9});
//        mAniList.Add(new List<int>(){17, 32, 6, 7, 8});
//        mAniList.Add(new List<int>(){16, 33});
//        mAniList.Add(new List<int>(){15, 34});
//        mAniList.Add(new List<int>(){14, 35});
//        mAniList.Add(new List<int>(){13, 36});
//        mAniList.Add(new List<int>(){12, 37});
//        
//        int nAniIndex = 0;
//        for(int i = 0; i < mAniList.Count; i++)
//        {
//            int nCurAniIndex = nAniIndex++;
//            List<int> mList = mAniList[i];
//            LeanTween.delayedCall(this.gameObject, 0.1f * nCurAniIndex, ()=>
//            {
//                foreach(int v in mList)
//                {
//                    GameObject go = mItemList[v - 1].gameObject;
//                    LeanTween.value(go, 0, 360, 0.5f).setOnUpdate((float fValue)=>
//                    {
//                        go.transform.localEulerAngles = new Vector3(fValue, 0, 0);
//                    });
//                }
//            });
//        }
//
//        LeanTween.delayedCall(gameObject, 2f, ()=>
//        {
//            int nAniIndex = 0;
//            for(int i = mAniList.Count - 1; i >= 0; i--)
//            {
//                int nCurAniIndex = nAniIndex++;
//                List<int> mList = mAniList[i];
//                LeanTween.delayedCall(this.gameObject, 0.1f * nCurAniIndex, ()=>
//                {
//                    foreach(int v in mList)
//                    {
//                        GameObject go = mItemList[v - 1].gameObject;
//                        LeanTween.value(go, 0, 360, 0.5f).setOnUpdate((float fValue)=>
//                        {
//                            go.transform.localEulerAngles = new Vector3(fValue, 0, 0);
//                        });
//                    }
//                });
//            }
//        });
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
//            AudioController.Instance.StopSound(Sounds.blast_bgm);
//            CancelLeanTween();
//            mItemList.Clear();
//
//            this.callBack();
//            this.callBack = null;
//        }
//    }
//
//    public void DoDestroyAction()
//    {   
//        if(this.gameObject != null)
//        {
//            mItemList.Clear();
//            AudioController.Instance.StopSound(Sounds.blast_bgm);
//            foreach(CardAnimationItem v in this.animationEntitys)
//            {
//                LeanTween.cancel(v.gameObject);
//                ResCenter.Instance.mCardAnimationItemPool.recycleObj(v);
//            }
//
//            this.animationEntitys = null;
//            Destroy(this.gameObject);
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