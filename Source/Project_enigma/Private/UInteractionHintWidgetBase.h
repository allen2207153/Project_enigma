// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UInteractionHintWidgetBase.generated.h"

/** ヒントUIの基底クラス
 *  Blueprintで継承して、実際の表示処理を実装する
 */
UCLASS()
class UUInteractionHintWidgetBase : public UUserWidget
{
	GENERATED_BODY()


public:
	/* 表示する文字列を設定*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "InteractionHint")
	void SetHintText(const FText& InText);

	/* 表示するアイコンを設定 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "InteractionHint")
	void SetHintIcon(UTexture2D* InIcon);
};

