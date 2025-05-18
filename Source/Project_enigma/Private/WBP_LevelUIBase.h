// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/Image.h>
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "WBP_LevelUIBase.generated.h"


UCLASS()

class UWBP_LevelUIBase : public UUserWidget{
	GENERATED_BODY()
	

public:
	// 設定鑽石數量
	UFUNCTION(BlueprintCallable)
	void SetDiamondCount(int32 Count);

	// 設定硬幣數量
	UFUNCTION(BlueprintCallable)
	void SetCoinCount(int32 CoinCount);

	// 設定時間（秒）
	UFUNCTION(BlueprintCallable)
	void SetTime(int32 Seconds);

protected:
	// 綁定三顆鑽石
	UPROPERTY(meta = (BindWidget))
	UImage* Diamond_0;

	UPROPERTY(meta = (BindWidget))
	UImage* Diamond_1;

	UPROPERTY(meta = (BindWidget))
	UImage* Diamond_2;

	// 硬幣數量
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;

	// 計時器
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	// 可以選擇設定灰色鑽石與亮色鑽石圖
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* DiamondInactive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* DiamondActive;
};
