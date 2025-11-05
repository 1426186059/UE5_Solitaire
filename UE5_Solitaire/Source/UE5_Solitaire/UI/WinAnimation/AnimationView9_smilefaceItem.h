//using System;
//using UnityEngine;
//using UnityEngine.U2D;
//using UnityEngine.UI;
//
//public class AnimationView9_smilefaceItem : MonoBehaviour 
//{
//    public GameObject goCenter;
//    public GameObject FanMian;
//    public GameObject ZhengMian;
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
//    public void SetLeftRight(int nPokerId1)
//    {
//        ZhengMian.GetComponent<Image>().sprite = GetSprite(nPokerId1);
//        FanMian.GetComponent<Image>().sprite = GetBackSprite();
//    }
//}