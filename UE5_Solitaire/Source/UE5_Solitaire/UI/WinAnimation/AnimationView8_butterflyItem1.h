//using System;
//using System.Collections.Generic;
//using Unity.Mathematics;
//using UnityEngine;
//using UnityEngine.U2D;
//using UnityEngine.UI;
//
//public class AnimationView8_butterflyItem1 : MonoBehaviour 
//{
//    public GameObject goCenter;
//    public GameObject left;
//    public GameObject right;
//    public GameObject leftBack;
//    public GameObject rightBack;
//
//    private AnimationView8_butterfly AnimationView8_butterfly;
//    public void Init(AnimationView8_butterfly AnimationView8_butterfly)
//    {
//        this.AnimationView8_butterfly = AnimationView8_butterfly;
//    }
//
//    private Sprite GetSprite(int nPokerId)
//    {
//        var cardDianshu = (nPokerId - 1) % 13 + 1;
//        var cardHuase = (nPokerId - 1) / 13  + 1;
//        int id = DataCenter.Instance.sysConfig.themeZhengId;
//        string path = "cards" + id;
//        SpriteAtlas atl_game = ResCenter.Instance.mBundleGameAllRes.GetAtlas(path);
//        string p_name = "di_" + cardDianshu + "_" + (int)cardHuase;
//        return atl_game.GetSprite(p_name);
//    }
//
//    private Sprite GetBackSprite()
//    {
//        SpriteAtlas atl_game1 = ResCenter.Instance.mBundleGameAllRes.GetAtlas(AtlasNames.Lobby_Game_Cards_Cardback);
//        string p_name_back = "cardback_" + DataCenter.Instance.sysConfig.themeBackId;
//        Sprite mBackSprite = atl_game1.GetSprite(p_name_back);
//        return mBackSprite;
//    }
//
//    public void SetLeftRight(int nPokerId1, int nPokerId2)
//    {
//        left.GetComponent<Image>().sprite = GetSprite(nPokerId1);
//        right.GetComponent<Image>().sprite = GetSprite(nPokerId2);
//        leftBack.GetComponent<Image>().sprite = GetBackSprite();
//        rightBack.GetComponent<Image>().sprite = GetBackSprite();
//    }
//
//    float fMinAngleY = 0;
//    float fMaxAngleY = 10;
//    public void FirstMove(int nIndex, bool bLeft)
//    {
//        this.bLeft = bLeft;
//        LeanTween.value(transform.gameObject, 0, 1f, 0.1f).setOnUpdateRatio((float fValue, float fTimePercent)=>
//        {
//            float fAngle = Mathf.Lerp(fMinAngleY, fMaxAngleY, fTimePercent);
//            leftBack.transform.localEulerAngles = new Vector3(0, -fAngle, 0);
//            rightBack.transform.localEulerAngles = new Vector3(0, fAngle, 0);
//        }).setLoopPingPong(-1);
//
//        LeanTween.value(transform.gameObject, 0, 1f, 1).setOnUpdateRatio((float fValue, float fTimePercent)=>
//        {
//            fMinAngleY = Mathf.Lerp(0, 40, fTimePercent);
//            fMaxAngleY = Mathf.Lerp(10, 80, fTimePercent);
//        });
//
//        CollectMyPos();
//        LeanTween.delayedCall(transform.gameObject, 1, ()=>
//        {
//            Vector3 targetScale = Vector3.one * (nIndex / 13f * 0.4f + 0.3f);
//            LeanTween.scale(transform.gameObject, targetScale, 1f);
//            
//            Vector3 targetPos = MyRightPosList[0];
//            if(this.bLeft)
//            {
//                targetPos = MyLeftPosList[0];
//            }
//            
//            var id = LeanTween.moveLocal(gameObject, targetPos, 0.5f).setOnComplete(()=>
//            {
//                DoMove(1);
//            }).id;
//            AnimationView8_butterfly.AddLeanTweenId(id);
//        });
//    }
//
//    public void DoMove(int nPosIndex)
//    {
//        Vector3 targetPos = MyRightPosList[nPosIndex % MyRightPosList.Count];
//        if(this.bLeft)
//        {
//            targetPos = MyLeftPosList[nPosIndex % MyLeftPosList.Count];
//        }
//        
//        float fTime = Vector3.Distance(targetPos, transform.localPosition) * 0.0025f;
//        var id = LeanTween.moveLocal(gameObject, targetPos, fTime).setOnComplete(()=>
//        {
//            DoMove(nPosIndex + 1);
//        }).id;
//        AnimationView8_butterfly.AddLeanTweenId(id);
//    }
//
//    private static List<Vector3> MyLeftPosList = null;
//    private static List<Vector3> MyRightPosList = null;
//    bool bLeft = false;
//    private void CollectMyPos()
//    {
//        int nSplitCount = 30;
//        if(MyLeftPosList == null)
//        {
//            MyLeftPosList = new List<Vector3>();
//            for(int i = 0; i < nSplitCount; i++)
//            {
//                float fAngle = i / (float)nSplitCount * 180;
//                Vector3 mPos = GetPos_NiShiZhen(fAngle, new Vector3(-Screen.width / 4f, 0, 0));
//                MyLeftPosList.Add(mPos);
//            }
//
//            for(int i = 0; i < nSplitCount; i++)
//            {
//                float fAngle = i / (float)nSplitCount * 180;
//                Vector3 mPos = GetPos_ShunShiZhen(fAngle, new Vector3(Screen.width / 4f, 0, 0));
//                MyLeftPosList.Add(mPos);
//            }
//        }
//
//        if(MyRightPosList == null)
//        {
//            MyRightPosList = new List<Vector3>();
//            for(int i = 0; i < nSplitCount; i++)
//            {
//                float fAngle = i / (float)nSplitCount * 180;
//                Vector3 mPos = GetPos_ShunShiZhen(fAngle, new Vector3(Screen.width / 4f, 0, 0));
//                MyRightPosList.Add(mPos);
//            }
//            
//            for(int i = 0; i < nSplitCount; i++)
//            {
//                float fAngle = i / (float)nSplitCount * 180;
//                Vector3 mPos = GetPos_NiShiZhen(fAngle, new Vector3(-Screen.width/ 4f, 0, 0));
//                MyRightPosList.Add(mPos);
//            }
//        }
//
//    }
//
//    private Vector3 GetPos_ShunShiZhen(float fAngle, Vector3 centerPos)
//    {   
//        fAngle = fAngle / 180f * Mathf.PI;
//        float fRadic = Screen.width / 4f;
//        float Y = fRadic * Mathf.Cos(fAngle);
//        if (fAngle >= Mathf.PI / 2f && fAngle <= Mathf.PI / 2f * 3)
//        {
//            Y = -Math.Abs(Y);
//        }
//        else
//        {
//             Y = Math.Abs(Y);
//        }
//
//        float X = fRadic * Mathf.Sin(fAngle);
//        return new Vector3(X, Y, 0) + centerPos;
//    }
//
//    private Vector3 GetPos_NiShiZhen(float fAngle, Vector3 centerPos)
//    {   
//        fAngle = fAngle / 180f * Mathf.PI;
//        float fRadic = Screen.width / 4f;
//        float Y = fRadic * Mathf.Cos(fAngle);
//        if (fAngle >= Mathf.PI / 2f && fAngle <= Mathf.PI / 2f * 3)
//        {
//            Y = -Math.Abs(Y);
//        }
//        else
//        {
//             Y = Math.Abs(Y);
//        }
//        
//        float X = -fRadic * Mathf.Sin(fAngle);
//        return new Vector3(X, Y, 0) + centerPos;
//    }
//
//}