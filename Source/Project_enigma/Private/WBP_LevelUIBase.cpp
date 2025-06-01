#include "WBP_LevelUIBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"


void UWBP_LevelUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	// 開始時隱藏非遊玩用 UI
	if (ResultRoot)
	{
		ResultRoot->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameOverRoot)
	{
		GameOverRoot->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UWBP_LevelUIBase::SetDiamondCount(int32 Count)
{
	Diamonds = Count;

	TArray<UImage*> DiamondsArray = { Diamond_0, Diamond_1, Diamond_2 };
	for (int32 i = 0; i < DiamondsArray.Num(); ++i)
	{
		if (DiamondsArray[i])
		{
			UTexture2D* Tex = (i < Count) ? DiamondActive : DiamondInactive;
			if (Tex)
			{
				DiamondsArray[i]->SetBrushFromTexture(Tex);
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

void UWBP_LevelUIBase::SetTime(float Seconds)
{
	if (TimerText)
	{
		int32 DisplaySeconds = FMath::FloorToInt(Seconds);
		FString TimeStr = FString::Printf(TEXT("Time: %03d"), DisplaySeconds);
		TimerText->SetText(FText::FromString(TimeStr));
	}
}

void UWBP_LevelUIBase::SetResultData(int32 InCoins, int32 InDiamonds, float InElapsedTime)
{
	Coins = InCoins;
	Diamonds = InDiamonds;
	ElapsedTime = InElapsedTime;
}

void UWBP_LevelUIBase::ShowResultScreen()
{
	if (ResultRoot)
	{
		ResultRoot->SetVisibility(ESlateVisibility::Visible);
	}
	if (InGameUIRoot)
	{
		InGameUIRoot->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ResultCoinText)
	{
		ResultCoinText->SetText(FText::AsNumber(Coins));
	}
	if (ResultTimeText)
	{
		ResultTimeText->SetText(FText::AsNumber(FMath::FloorToInt(ElapsedTime)));
	}

	SetDiamondCount(Diamonds);
}

void UWBP_LevelUIBase::HideResultScreen()
{
	if (ResultRoot)
	{
		ResultRoot->SetVisibility(ESlateVisibility::Hidden);
	}
	if (InGameUIRoot)
	{
		InGameUIRoot->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWBP_LevelUIBase::ShowGameOverScreen()
{
	if (GameOverRoot)
	{
		GameOverRoot->SetVisibility(ESlateVisibility::Visible);
	}
	if (InGameUIRoot)
	{
		InGameUIRoot->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWBP_LevelUIBase::HideGameOverScreen()
{
	if (GameOverRoot)
	{
		GameOverRoot->SetVisibility(ESlateVisibility::Hidden);
	}
	if (InGameUIRoot)
	{
		InGameUIRoot->SetVisibility(ESlateVisibility::Visible);
	}
}