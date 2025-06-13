// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_TitleScreen.generated.h"

class UWBP_TitleScreen;


UCLASS()
class AGameMode_TitleScreen : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// WBP_TitleScreen の Blueprint クラスを設定可能にする
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UWBP_TitleScreen> TitleWidgetClass;

	// 実際のウィジェットインスタンス
	UPROPERTY()
	UWBP_TitleScreen* TitleWidget;
};
