#pragma once

#include "UE5_Solitaire/UI/Item/PokerAnimationItemW.h"
#include "UE5_Solitaire/SimpleFramework/KKSFHead.h"
#include "CoreMinimal.h"
#include "AnimationView2_Default_Widget.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UAnimationView2_Default_Widget : public UWUIBase
{
    GENERATED_BODY()

protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
    virtual void OnCreate() override;
    virtual void Init() override;
    virtual void Show() override;
    virtual void Hide(bool bDestroy) override;
    virtual void OnLayoutChanged() override;
    virtual void CheckFirstLayoutOkToShow() override;
public:
    class AnimationEntity
    {
    public:
        UWidget* firstNode = nullptr;
        
        int nDigitId = 13;
        int nColor = 0;
        int nPokerId = 1;  //13*v+value cardid
        
        int index = 0; //第几个col的
        bool open = false;
        bool trigger = false;
        float triggerDelay = 0;
        bool btoRight = true;
        float deltTime = 0;
        float checktimes = 0; //每两帧，检查一次位置。

        Vector3 startPt = new Vector3(0, 0, 0);  //最开始的起始点。
        Vector3 nowPt = new Vector3(0, 0, 0);

        float vx = 0;    // x方向的速度
        float vy = 0;    // y方向的速度  
        float vyMax = 0; // y方向的最大速度
        float vx_a = 0;  // x方向的加速度 x轴匀速
        float vy_a = 0;  // y方向的加速度
        float maxHeight = 0;   //每次更新最高值。

        static bool toRight(int index)
        {
            TArray<int32> toRightRate = { 100, 50, 40, 30 };
            int rate = KKRandomTool::RandomInt(0, 100);
            if (DataCenter.Instance.sysConfig.leftHand == 1)
            {
                rate = KKRandomTool::RandomInt(0, 200);
            }
            bool toRight = rate <= toRightRate[index];
            return toRight;
        }

        static float randomVx()
        {
            return KKRandomTool::RandomInt(200, 260);
        }

        static float randomVy()
        {
            return KKRandomTool::RandomInt(500, 600);
        }

        static float randomVy_a()
        {
            return KKRandomTool::RandomInt(5500, 6200) * (-1);
        }

        static int32 getCardId(int32 nDigitId, int32 color)
        {
            return (int32)nDigitId * 13 + color;
        }

        static AnimationEntity create(int32 nDigitId, int32 color, int32 value)
        {
            AnimationEntity entity = new AnimationEntity();
            entity.color = color;
            entity.index = index;
            entity.value = value;
            entity.cardid = AnimationEntity.getCardId(color, value);
            return entity;
        }
    };
      
    // Card 数据
    const float CardWidth = 103;
    const float CardHeigt = 154;
    const float CardsColTotal = 120; //每个数字最多显示多少张card，避免过多导致卡顿。
    const float Delay_Col_Offset = 0.1f; //5*2/30
    const float Delay_Value_Offset = 0.01f; // 22*2/30

    UWidget* skipNode = nullptr;
    TFunction<void()> callBack = nullptr;
    FVector2D animationSize = new FVector2D(0, 0);

    float maxHeight = 0;
    float minHeight = 0;
    float maxWidth = 0;
    float minWidth = 0;
    
    TMap<int, TSArray<CardAnimationItem>> colNodes_Dic = new TMap<int, TSArray<CardAnimationItem>>();
    TArray<CardAnimationItem> allNodes = new TArray<CardAnimationItem>();
    TArray<AnimationEntity> animationEntitys = new TArray<AnimationEntity>();
    bool animationOver = false;
    RectTransform cardsNode = null;
    TArray<CardType> colors = new TArray<CardType>();
    AudioSource mBlastBgm = null;
    
    void Awake();
    void Show(TFunction<void()> callback);
    void showAnimation(TSArray<CardType> colors, Vector3 startPt_w, int offsetX = 0);
    void showAnimation_Default(Vector3 pt, TSArray<CardType> colors, int offsetX);
    GameObject addStaticCard(Vector3 pt, CardType colorType, int value, int colindex);
    void updateAnimation(AnimationEntity entity, float dt);
    void Update();
    void onAnimatinCallBack();
    void DoDestroyAction();
    void onClick_Skip();
}