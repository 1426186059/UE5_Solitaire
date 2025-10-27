#include "ResCenter.h"

void ResCenter::Init()
{
	
}

UPaperSprite* ResCenter::GetPokerSprite(int nPokerId)
{
    if (nPokerId > 0)
    {
        int nDigetId = CardHandler::GetSingleton()->GetDigital(nPokerId);
        int nSubDigetId = CardHandler::GetSingleton()->GetSubDigital(nPokerId);
        int themeZhengId = ThemeDataHandler::GetSingleton()->data->themeZhengId;

        FString resPath = FString::Printf(
            TEXT("/Game/ResourceABs/MainScene/UI/AtlasGroup/poker/card_%d/Frames/card_%d_di_%d_%d_png.card_%d_di_%d_%d_png"),
            themeZhengId, themeZhengId, nDigetId, nSubDigetId, themeZhengId, nDigetId, nSubDigetId);
        
        UPaperSprite* PokerSprite = LoadObject<UPaperSprite>(nullptr, *resPath);
        return PokerSprite;
    }
    else
    {
        int themeBackId = ThemeDataHandler::GetSingleton()->data->themeBackId;
        FString resPath = FString::Printf(
            TEXT("/Game/ResourceABs/MainScene/UI/AtlasGroup/poker/cardback/Frames/cardback_cardback_%d_png.cardback_cardback_%d_png"),
            themeBackId, themeBackId);

        UPaperSprite* PokerSprite = LoadObject<UPaperSprite>(nullptr, *resPath);
        return PokerSprite;
    }
}


