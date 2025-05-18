// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectBase.h"
#include "IA_ToggleObject.generated.h"

/**
 * 
 */
UCLASS()
class AIA_ToggleObject : public AObjectBase
{
	GENERATED_BODY()
	
public:
	AIA_ToggleObject();

protected:
	virtual void BeginPlay() override;

public:
	/** 互動処理（仮想カーソル経由のみ） */
	virtual void OnInteract_Implementation() override;

	/** トグル動作の目標位置／回転 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle")
	FTransform TargetTransform;

	/** 補間時間（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle")
	float ToggleLerpTime = 1.0f;

	/** 移動か回転か？（true = 移動, false = 回転） ※Blend方式により自動対応 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle")
	bool bToggleMove = true;

private:
	/** 元の Transform（初期位置保存用） */
	FTransform OriginalTransform;

	/** 今目標位置にいるか？ */
	bool bIsAtTarget = false;

	/** 現在トグル中か？ */
	bool bIsToggling = false;

	/** 補間用タイマー（再入力対応） */
	FTimerHandle ToggleTimerHandle;
};
