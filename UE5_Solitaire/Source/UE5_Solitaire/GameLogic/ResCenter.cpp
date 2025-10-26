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

        FString resPath = FString::Printf(
            TEXT("/Game/ResourceABs/MainScene/UI/AtlasGroup/poker/%s/Frames/card_1_di_%d_%d_png.card_1_di_%d_%d_png"),
            TEXT(""), nDigetId, nSubDigetId, nDigetId, nSubDigetId);
           
        UPaperSprite* PokerSprite = LoadObject<UPaperSprite>(nullptr, *resPath);
        return PokerSprite;
    }
    else
    {
       // int themeBackId = ThemeDataHandler.data.themeBackId;
        FString resPath = FString::Printf(
            TEXT("/Game/ResourceABs/MainScene/UI/AtlasGroup/poker/%s/Frames/card_1_di_%d_%d_png.card_1_di_%d_%d_png"),
            TEXT(""), 0, 0, 0, 0);

        UPaperSprite* PokerSprite = LoadObject<UPaperSprite>(nullptr, *resPath);
        return PokerSprite;
    }
}


