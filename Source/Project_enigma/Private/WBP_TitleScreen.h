// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_TitleScreen.generated.h"

/**
 * 
 */
UCLASS()
class UWBP_TitleScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Aボタンでメニューを表示する関数
	UFUNCTION(BlueprintCallable, Category = "Title")
	void ShowMenu();

protected:
	// ===== ウィジェット参照 =====
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Hinttext;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* playbutton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* optionbutton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* quitbutton;

	// ===== プレイ時に遷移するレベル名（BPで設定可能） =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title")
	FName LevelToLoad = NAME_None;

	// ===== 呼吸エフェクト用 =====
	float BreathTime = 0.0f;
	bool bMenuShown = false;

	// ===== ボタン処理設定 =====
	void SetupButtonEvents();

	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnQuitClicked();
};