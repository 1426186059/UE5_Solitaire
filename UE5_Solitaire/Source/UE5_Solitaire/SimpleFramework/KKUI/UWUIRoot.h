#pragma once

#include "UMGHelper.h"
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UWUIRoot.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UWUIRoot:public UUserWidget
{
	GENERATED_BODY()
public:
	void Init();

	UCanvasPanel* Layer_Background;
	UCanvasPanel* Layer_Base;
	UCanvasPanel* Layer_Loading;
	UCanvasPanel* Layer_Popup;
	UCanvasPanel* Layer_Tip;
private:
	bool bInit;
};
