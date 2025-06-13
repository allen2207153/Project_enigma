
#pragma once

#include "CoreMinimal.h"
#include <Components/CanvasPanel.h>
#include <Components/Image.h>
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "WBP_LevelUIBase.generated.h"


UCLASS()

class UWBP_LevelUIBase : public UUserWidget{
	GENERATED_BODY()
	

public:
	// ========== UI Root Panels ==========
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* InGameUIRoot;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* ResultRoot;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* GameOverRoot;

	// ========== 結算數據用 ==========
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultCoinText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultTimeText;

	/** 再挑戦ボタン（ステージをリスタート）*/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* ReplayButton;

	/** 次のステージへ（仮機能、未実装でもOK）*/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* NextStageButton;

	/** メインメニューに戻る（仮機能、未実装でもOK）*/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UButton* ReturnToMenuButton;

	// ========== 函数 ==========
	UFUNCTION(BlueprintCallable)
	void ShowResultScreen();

	UFUNCTION(BlueprintCallable)
	void HideResultScreen();

	UFUNCTION(BlueprintCallable)
	void ShowGameOverScreen();

	UFUNCTION(BlueprintCallable)
	void HideGameOverScreen();

	void NativeConstruct();

	void SetDiamondCount(int32 Count);

	void SetCoinCount(int32 CoinCount);

	void SetTime(float Seconds);

	void SetResultDiamondCount(int32 Count);

	void SetResultData(int32 InCoins, int32 InDiamonds, float InElapsedTime);


	// ===== ボタン処理設定 =====
	void SetupButtonEvents();

	UFUNCTION(BlueprintCallable)
	void OnReplayButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnNextStageButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnReturnToMenuButtonClicked();

	/** 次のステージのレベル名（Blueprint で設定可） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName NextStageLevelName;

	/** タイトル画面のレベル名（Blueprint で設定可） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName TitleScreenLevelName;
protected:
	// ダイヤ画像
	UPROPERTY(meta = (BindWidget))
	UImage* Diamond_0;

	UPROPERTY(meta = (BindWidget))
	UImage* Diamond_1;

	UPROPERTY(meta = (BindWidget))
	UImage* Diamond_2;

	// 結果畫面用のダイヤ画像
	UPROPERTY(meta = (BindWidget))
	UImage* ResultDiamond_0;

	UPROPERTY(meta = (BindWidget))
	UImage* ResultDiamond_1;

	UPROPERTY(meta = (BindWidget))
	UImage* ResultDiamond_2;

	// コイン数
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinText;

	// タイムカウント
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

	// 可以選擇設定灰色鑽石與亮色鑽石圖
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* DiamondInactive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* DiamondActive;


	

private:
	// 保存的數據（UI 內部用）
	int32 Coins = 0;
	int32 Diamonds = 0;
	float ElapsedTime = 0.0f;
};
