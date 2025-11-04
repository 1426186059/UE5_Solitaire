
#include "GameWinAniMgr.h"


void UGameWinAniMgr::showAnimation(SolitaireWinAnimationType at, TArray<int32> colors, FVector2D startPt_w, int offsetX, TFunction<void()> callback = nullptr)
{
    this->nAniType = at;
    if (at == AnimationType.Default)
    {
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation2");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView2_Default>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
    }
    else if (at == AnimationType.FlyFullScreen)
    {
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation5");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView5_FlyFullScreen>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
    }
    else if (at == AnimationType.FiveStar)
    {
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation6_5Star");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView6_5Star>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(at, colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
    }
    else if (at == AnimationType.LingXing)
    {
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation7_LingXing");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView7_LingXing>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(at, colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
    }
    else if (at == AnimationType.Butterfly)
    {
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation8_butterfly");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView8_butterfly>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(at, colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
    }
    else if (at == AnimationType.SmileFace)
    {
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation9_SmileFace");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView9_smileface>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(at, colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
    }
    else
    {
        PrintTool.LogError("Sorry, There is No Animation: ", this.nAniType);
    }

    if (this.animationNode == null)
    {
        //如果某些动画没初始化好，那就使用默认的动画
        var prefab = ResCenter.Instance.mBundleGameAllRes.FindPrefab("gameanimation2");
        GameObject node = Instantiate(prefab) as GameObject;
        node.transform.SetParent(GameLauncher.readOnlyInstance.mUIRoot.mCanvas_WinAnimation, false);
        var mAnimationView = node.GetComponent<AnimationView2_Default>();
        mAnimationView.gameObject.SetActive(true);
        mAnimationView.Show(colors, startPt_w, offsetX, callback);
        this.animationNode = mAnimationView.gameObject;
        this.nAniType = AnimationType.Default;
    }
}
            
    public void RemoveLastWinAniNode()
    {
        if (this.animationNode)
        {
            if (this.nAniType == AnimationType.Default)
            {
                this.animationNode.GetComponent<AnimationView2_Default>().DoDestroyAction();
            }
            else if (this.nAniType == AnimationType.FlyFullScreen)
            {
                this.animationNode.GetComponent<AnimationView5_FlyFullScreen>().DoDestroyAction();
            }
            else if (this.nAniType == AnimationType.FiveStar)
            {
                this.animationNode.GetComponent<AnimationView6_5Star>().DoDestroyAction();
            }
            else if (this.nAniType == AnimationType.LingXing)
            {
                this.animationNode.GetComponent<AnimationView7_LingXing>().DoDestroyAction();
            }
            else if (this.nAniType == AnimationType.Butterfly)
            {
                this.animationNode.GetComponent<AnimationView8_butterfly>().DoDestroyAction();
            }
            else if (this.nAniType == AnimationType.SmileFace)
            {
                animationNode.GetComponent<AnimationView9_smileface>().DoDestroyAction();
            }
            else
            {
                PrintTool.Assert(false, "RemoveLastWinAniNode Error");
            }
            this.animationNode = null;
        }
    }

}



