// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectBase.generated.h"


class USoundBase;
class UParticleSystem;
class UAnimationAsset;
class USkeletalMeshComponent;





// オブジェクトのタイプ（動作フラグ）
UENUM(BlueprintType)
enum class EObjectType : uint8
{
	None            UMETA(DisplayName = "None"),             // 通常オブジェクト
	Interactable    UMETA(DisplayName = "Interactable"),     // プレイヤーが操作可能
	AutoRotating    UMETA(DisplayName = "Auto Rotating"),    // 自動回転
};

UCLASS()
class AObjectBase : public AActor
{
	GENERATED_BODY()



public:
	AObjectBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	/** プレイヤーがインタラクト可能かどうか（UI表示や操作トリガー用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bIsPlayerInteractable = false;

	/** UI Widget からのみ操作される対象か（バーチャルカーソル連携など） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bOnlyUsableFromWidget = false;

	/** このオブジェクトは UI Widget 専用かどうか（C++ や Blueprint で利用可能） */
	UFUNCTION(BlueprintCallable, Category = "Object")
	bool IsWidgetOnlyObject() const;

	/** 自動処理を開始するか（BeginPlay 時に起動） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bIsAutoExecute = false;

	/** オブジェクトの分類タイプ（ギミック分岐や判定用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	EObjectType ObjectType = EObjectType::None;

	// ======================
	// ▼ インタラクト関連イベント
	// ======================

	/** プレイヤーによるインタラクト時に呼ばれる（バーチャルカーソルやキャラ接近で使用） */
	UFUNCTION(BlueprintNativeEvent, Category = "Object")
	void OnInteract();
	virtual void OnInteract_Implementation();

	/** 自動実行処理（例：回転など） */
	UFUNCTION(BlueprintNativeEvent, Category = "Object")
	void OnAutoExecute();
	virtual void OnAutoExecute_Implementation();

protected:
	// ======================
	// ▼ メッシュ構造
	// ======================

	/** 表示用スタティックメッシュ（ルート） */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	/** アニメ再生用スケルタルメッシュ（任意） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Visual")
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	// ======================
	// ▼ サウンド・エフェクト・アニメーション
	// ======================

	/** インタラクト時に再生される音 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	USoundBase* InteractSound = nullptr;

	/** インタラクト時に発生するパーティクル */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	UParticleSystem* InteractEffect = nullptr;

	/** アニメーション再生（スケルタルメッシュが必要） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	UAnimationAsset* InteractAnimation = nullptr;

	/** エフェクトの位置調整（発生オフセット） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	FVector EffectOffset = FVector::ZeroVector;

	// ======================
	// ▼ 自動回転設定
	// ======================

	/** 一回の回転量（例：Yaw 90度） */
	UPROPERTY(EditAnywhere, Category = "AutoRotation")
	FRotator RotationSpeed = FRotator(0.f, 90.f, 0.f);

	/** 自動回転の実行間隔（秒） */
	UPROPERTY(EditAnywhere, Category = "AutoRotation")
	float AutoExecuteInterval = 2.0f;

	/** 回転補間速度（度/秒） */
	UPROPERTY(EditAnywhere, Category = "SmoothRotation")
	float RotationSpeedDegPerSec = 180.f;

private:
	// タイマー用ハンドル（自動実行）
	FTimerHandle AutoExecuteTimerHandle;

	// 回転中かどうかのフラグ
	bool bIsRotating = false;

	// 回転目標角度
	FRotator TargetRotation;

	// スムーズな回転処理を開始する
	void StartSmoothRotation();
};