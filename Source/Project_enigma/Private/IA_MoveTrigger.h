// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectBase.h"
#include "IA_MoveTrigger.generated.h"

/**
 * 
 */
UCLASS()
class AIA_MoveTrigger : public AObjectBase
{
	GENERATED_BODY()
	
public:
	AIA_MoveTrigger();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation() override;

	void DeferredDestroy();

public:
	// ===== 設定可能なプロパティ =====

	/* 移動対象のアクター（例：足場など） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	AActor* TargetToMove = nullptr;

	/* 移動のオフセット量（例：上方向に100cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	FVector MoveOffset = FVector(0, 0, 100);

	/* インタラクト後にこのトリガー自体を削除するか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	bool bDestroyAfterInteract = true;

	/* 移動速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	float MoveSpeed = 300.f;

private:
	// ===== 内部制御用 =====

	/* 削除用のタイマーハンドル */
	FTimerHandle DestroyTimerHandle;

	/* 現在移動中かどうかのフラグ */
	bool bShouldMove = false;

	/* 計算された目標座標 */
	FVector MoveTargetLocation;
};

