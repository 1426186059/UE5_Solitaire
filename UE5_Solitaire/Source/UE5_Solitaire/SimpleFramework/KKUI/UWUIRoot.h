#pragma once

#include "UWUIBase.h"
#include "Blueprint/UserWidget.h"

#include "CoreMinimal.h"
#include "UWUIRoot.generated.h"

UCLASS()
class UE5_SOLITAIRE_API UWUIRoot : public UWUIBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UCanvasPanel* Layer_Background;
	UCanvasPanel* Layer_Base;
	UCanvasPanel* Layer_Loading;
	UCanvasPanel* Layer_Popup;
	UCanvasPanel* Layer_Tip;
private:
	bool bInit;
};
