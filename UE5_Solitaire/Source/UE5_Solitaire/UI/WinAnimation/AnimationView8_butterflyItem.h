using System;
using UnityEngine;
using UnityEngine.U2D;
using UnityEngine.UI;

public class AnimationView8_butterflyItem : MonoBehaviour
{
    public GameObject goCenter;
    public GameObject left;
    public GameObject right;
    public GameObject leftBack;
    public GameObject rightBack;

    private AnimationView8_butterfly AnimationView8_butterfly;
    public void Init(AnimationView8_butterfly AnimationView8_butterfly)
    {
        this.AnimationView8_butterfly = AnimationView8_butterfly;
    }

    private Sprite GetSprite(int nPokerId)
    {
        var cardDianshu = (nPokerId - 1) % 13 + 1;
        var cardHuase = (nPokerId - 1) / 13 + 1;

        int id = DataCenter.Instance.sysConfig.themeZhengId;
        string path = "cards" + id;
        SpriteAtlas atl_game = ResCenter.Instance.mBundleGameAllRes.GetAtlas(path);

        string p_name = "di_" + cardDianshu + "_" + (int)cardHuase;
        return atl_game.GetSprite(p_name);
    }

    private Sprite GetBackSprite()
    {
        SpriteAtlas atl_game1 = ResCenter.Instance.mBundleGameAllRes.GetAtlas(AtlasNames.Lobby_Game_Cards_Cardback);
        string p_name_back = "cardback_" + DataCenter.Instance.sysConfig.themeBackId;
        Sprite mBackSprite = atl_game1.GetSprite(p_name_back);
        return mBackSprite;
    }

    public void SetLeftRight(int nPokerId1, int nPokerId2)
    {
        left.GetComponent<Image>().sprite = GetSprite(nPokerId1);
        right.GetComponent<Image>().sprite = GetSprite(nPokerId2);
        leftBack.GetComponent<Image>().sprite = GetBackSprite();
        rightBack.GetComponent<Image>().sprite = GetBackSprite();
    }

    public Vector3 GetRandomPos()
    {
        Vector2 ScreenSize = GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.rect.size;
        float fMaxX = ScreenSize.x / 2f - 60;
        float fMinX = -ScreenSize.x / 2f + 60;
        float fMaxY = ScreenSize.y / 2f - 300;
        float fMinY = -ScreenSize.y / 2f + 400;
        float fPosX = UnityEngine.Random.Range(fMinX, fMaxX);
        float fPosY = UnityEngine.Random.Range(fMinY, fMaxY);
        return new Vector3(fPosX, fPosY, 0);
    }

    float fMinAngleY = 0;
    float fMaxAngleY = 10;
    public void FirstMove()
    {
        LeanTween.delayedCall(transform.gameObject, 1, () =>
        {
            this.DoMove();

            Vector3 targetScale = Vector3.one * UnityEngine.Random.Range(0.3f, 0.7f);
            LeanTween.scale(transform.gameObject, targetScale, 1f);
        });

        LeanTween.value(transform.gameObject, 0, 1f, 0.1f).setOnUpdateRatio((float fValue, float fTimePercent) =>
        {
            float fAngle = Mathf.Lerp(fMinAngleY, fMaxAngleY, fTimePercent);
            leftBack.transform.localEulerAngles = new Vector3(0, -fAngle, 0);
            rightBack.transform.localEulerAngles = new Vector3(0, fAngle, 0);
        }).setLoopPingPong(-1);

        LeanTween.value(transform.gameObject, 0, 1f, 1).setOnUpdateRatio((float fValue, float fTimePercent) =>
        {
            fMinAngleY = Mathf.Lerp(0, 40, fTimePercent);
            fMaxAngleY = Mathf.Lerp(10, 80, fTimePercent);
        });
    }

    public void DoMove()
    {
        float fTargetRotateZ = UnityEngine.Random.Range(-80f, 80f);
        float fTargetRotateY = UnityEngine.Random.Range(-80f, 80f);
        float fTargetRotateX = UnityEngine.Random.Range(-80f, 80f);
        Vector3 fTargetRotate = new Vector3(fTargetRotateX, fTargetRotateY, fTargetRotateZ);
        LeanTween.rotate(goCenter, fTargetRotate, 1f);

        Vector2 ScreenSize = GameLauncher.Instance.mUIRoot.mCanvas_WinAnimation.rect.size;
        Vector3 targetPos = GetRandomPos();
        var id = LeanTween.moveLocal(gameObject, targetPos, 1f).setOnComplete(() =>
        {
            var id1 = LeanTween.delayedCall(gameObject, 2f, () =>
            {
                DoMove();
            }).id;
            AnimationView8_butterfly.AddLeanTweenId(id1);
        }).id;
        AnimationView8_butterfly.AddLeanTweenId(id);
    }

    private Vector3 GetPos(float fAngle, Vector3 centerPos)
    {
        fAngle = fAngle / 180 * Mathf.PI;
        float fRadic = Screen.width / 4f;
        float Y = fRadic * Mathf.Cos(fAngle);
        float X = fRadic * Mathf.Sin(fAngle);
        return new Vector3(X, Y, 0) + centerPos;
    }

}