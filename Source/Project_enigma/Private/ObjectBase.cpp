#include "ObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimationAsset.h"
#include "TimerManager.h"
#include "Engine/World.h"

// ===============================
// コンストラクタ：初期値設定
// ===============================
AObjectBase::AObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// メッシュをルートに設定（見た目のオブジェクト）
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// デフォルト値
	ObjectType = EObjectType::None;
	RotationSpeed = FRotator(0.f, 90.f, 0.f);      // 回転量（1回あたり）
	AutoExecuteInterval = 2.0f;                    // 自動実行の間隔（秒）
	RotationSpeedDegPerSec = 180.f;                // 回転スピード（角度/秒）

	bIsPlayerInteractable = false;
	bOnlyUsableFromWidget = false;
}

// ===============================
// ゲーム開始時の初期化
// ===============================
void AObjectBase::BeginPlay()
{
	Super::BeginPlay();

	// ✅ バーチャルカーソルでのレイキャストに必要な設定
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		Mesh->SetGenerateOverlapEvents(true);
	}

	// 自動実行が有効で、間隔が正の値ならタイマー登録
	if (bIsAutoExecute && AutoExecuteInterval > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AutoExecuteTimerHandle,
			this,
			&AObjectBase::StartSmoothRotation,
			AutoExecuteInterval,
			true // ループ
		);
	}
}

// ===============================
// 毎フレームの更新（回転処理）
// ===============================
void AObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotating)
	{
		// 現在の角度から目標角度に向けて一定速度で補間
		FRotator Current = GetActorRotation();
		FRotator NewRotation = FMath::RInterpConstantTo(Current, TargetRotation, DeltaTime, RotationSpeedDegPerSec);

		SetActorRotation(NewRotation);

		// 回転完了判定（ほぼ一致したら終了）
		if (NewRotation.Equals(TargetRotation, 0.1f))
		{
			SetActorRotation(TargetRotation); // 最終補正
			bIsRotating = false;
		}
	}
}

// ===============================
// インタラクト処理（プレイヤー操作時）
// ===============================
void AObjectBase::OnInteract_Implementation()
{
	// デバッグログ（任意）
	UE_LOG(LogTemp, Log, TEXT("🔹 Object Interacted: %s"), *GetName());

	// サウンド再生
	if (InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
	}

	// パーティクル再生（エフェクト位置は少しオフセット可能）
	if (InteractEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InteractEffect, GetActorLocation() + EffectOffset);
	}

	// アニメーション再生（SkeletalMeshが設定されていれば）
	if (SkeletalMesh && InteractAnimation)
	{
		SkeletalMesh->PlayAnimation(InteractAnimation, false);
	}
}

// ===============================
// 自動実行用の処理（Override用）
// ===============================
void AObjectBase::OnAutoExecute_Implementation()
{
	// 派生クラスで自由に定義可能
}

// ===============================
// スムーズな回転開始
// ===============================
void AObjectBase::StartSmoothRotation()
{
	// すでに回転中なら無視
	if (bIsRotating) return;

	// 回転目標を現在の角度＋RotationSpeedに設定
	TargetRotation = GetActorRotation() + RotationSpeed;
	bIsRotating = true;
}

// ===============================
// UI Widget 用の専用判定関数
// ===============================
bool AObjectBase::IsWidgetOnlyObject() const
{
	return bOnlyUsableFromWidget;
}