#include "ResCenter.h"

void AResCenter::Init()
{
	
}

UPaperSprite* AResCenter::GetPokerSprite(int nPokerId)
{
    if (nPokerId > 0)
    {
        //Game/ResourceABs/MainScene/UI/AtlasGroup/poker/card1/Frames/card_1_di_10_1_png.card_1_di_10_1_png'
        int nDigetId = CardHandler::GetSingleton()->GetDigital(nPokerId);
        int nSubDigetId = CardHandler::GetSingleton()->GetSubDigital(nPokerId);
        int themeZhengId = ThemeDataHandler::GetSingleton()->data->themeZhengId;

        FString resPath = FString::Printf(
            TEXT("/Game/ResourceABs/MainScene/UI/AtlasGroup/poker/card%d/Frames/card_%d_di_%d_%d_png.card_%d_di_%d_%d_png"),
            themeZhengId, themeZhengId, nDigetId, nSubDigetId, themeZhengId, nDigetId, nSubDigetId);
        
        UPaperSprite* PokerSprite = LoadObject<UPaperSprite>(nullptr, *resPath);
        ensureMsgf(PokerSprite, TEXT("AResCenter PokerSprite Error: %s"), *resPath);
        ensureMsgf(PokerSprite && PokerSprite->IsValidLowLevel(), TEXT("AResCenter PokerSprite Error: %s"), *resPath);
        return PokerSprite;
    }
    else
    {
        //Game/ResourceABs/MainScene/UI/AtlasGroup/poker/cardback/Frames/cardback_cardback_10_png.cardback_cardback_10_png'
        int themeBackId = ThemeDataHandler::GetSingleton()->data->themeBackId;
        FString resPath = FString::Printf(
            TEXT("/Game/ResourceABs/MainScene/UI/AtlasGroup/poker/cardback/Frames/cardback_cardback_%d_png.cardback_cardback_%d_png"),
            themeBackId, themeBackId);

        UPaperSprite* PokerSprite = LoadObject<UPaperSprite>(nullptr, *resPath);
        return PokerSprite;
    }
}


