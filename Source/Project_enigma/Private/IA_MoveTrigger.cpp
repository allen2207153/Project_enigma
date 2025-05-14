#include "IA_MoveTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"


AIA_MoveTrigger::AIA_MoveTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	// オフセット距離（移動方向と距離を指定）
	MoveOffset = FVector(0, 0, 150);  // Z方向に上昇
	MoveSpeed = 300.f;                // 移動スピード

	// インタラクト後に自身を削除するかどうか
	bDestroyAfterInteract = true;

	// オブジェクトタイプをインタラクト可能に設定
	ObjectType = EObjectType::Interactable;
}

void AIA_MoveTrigger::BeginPlay()
{
	Super::BeginPlay();
}

// ========== 毎フレーム実行：移動処理 ==========
void AIA_MoveTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 移動フラグがON、かつターゲットが存在する場合
	if (bShouldMove && TargetToMove)
	{
		FVector Current = TargetToMove->GetActorLocation();

		// 目的地に向けて移動（直線補間）
		FVector NewLocation = FMath::VInterpConstantTo(Current, MoveTargetLocation, DeltaTime, MoveSpeed);
		TargetToMove->SetActorLocation(NewLocation);

		// 十分に近づいたら移動完了として補正＆停止
		if (FVector::Dist(NewLocation, MoveTargetLocation) < 1.f)
		{
			TargetToMove->SetActorLocation(MoveTargetLocation); 
			bShouldMove = false;
		}
	}
}

// ========== インタラクト処理（プレイヤーが起動） ==========
void AIA_MoveTrigger::OnInteract_Implementation()
{
	Super::OnInteract_Implementation();

	// トリガー自身のメッシュを非表示＆衝突無効にする
	if (Mesh)
	{
		Mesh->SetVisibility(false, true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// ターゲットオブジェクトの移動開始（目標地点 = 現在位置 + オフセット）
	if (TargetToMove)
	{
		MoveTargetLocation = TargetToMove->GetActorLocation() + MoveOffset;
		bShouldMove = true;
	}

	// 自動削除が有効な場合、0.5秒後に自身を破棄
	if (bDestroyAfterInteract)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DestroyTimerHandle,
			this,
			&AIA_MoveTrigger::DeferredDestroy,
			0.5f,
			false
		);
	}
}

// ========== 遅延削除処理 ==========
void AIA_MoveTrigger::DeferredDestroy()
{
	Destroy();
}