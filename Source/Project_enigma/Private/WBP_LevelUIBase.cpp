#include "WBP_LevelUIBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWBP_LevelUIBase::SetDiamondCount(int32 Count)
{
	TArray<UImage*> Diamonds = { Diamond_0, Diamond_1, Diamond_2 };

	for (int32 i = 0; i < Diamonds.Num(); ++i)
	{
		if (Diamonds[i])
		{
			UTexture2D* Tex = (i < Count) ? DiamondActive : DiamondInactive;
			if (Tex)
			{
				Diamonds[i]->SetBrushFromTexture(Tex);
			}
		}
	}
}

void UWBP_LevelUIBase::SetCoinCount(int32 CoinCount)
{
	if (CoinText)
	{
		CoinText->SetText(FText::AsNumber(CoinCount));
	}
}

void UWBP_LevelUIBase::SetTime(int32 Seconds)
{
	if (TimerText)
	{
		FString TimeStr = FString::Printf(TEXT("Time: %03d"), Seconds);
		TimerText->SetText(FText::FromString(TimeStr));
	}
}