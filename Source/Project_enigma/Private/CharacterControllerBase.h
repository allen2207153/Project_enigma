// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterControllerBase.generated.h"

class UInputMappingContext;
class UInputAction;
class ACharacterBase;
class ACameraCenterActor;
struct FInputActionValue;

class UWBP_LevelUIBase;


UCLASS()
class ACharacterControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	// ========== UI関連 ==========
	// UI (BP編集)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UWBP_LevelUIBase> LevelUIClass;

	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWBP_LevelUIBase> LevelUIInstance;
	// ========== 入力関連（Enhanced Input） ==========

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CurrentMappingContext;

	// 移動入力（スティック・WASD）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	// カメラ視点移動（右スティック・マウス）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	// ズーム切り替え（例：L2トリガー）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomAction;

	// ダッシュ（例：Bボタン長押し）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RunAction;

	// インタラクト（例：Aボタン）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	// バーチャルカーソルON/OFF（例：L1トリガー）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleCursorAction;


	TObjectPtr<ACharacterBase> CurrentCharacter;

	TObjectPtr<ACameraCenterActor> CameraCenterActor;

	// 時間計時（秒）
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	int32 ElapsedSeconds = 0;

	// Timer 用 Handle（管理器）
	FTimerHandle TimerHandle_UpdateTime;

	// 崖の落下チェック距離（前方）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeGuard")
	float GroundForwardCheckDistance = 50.0f;

	// 崖の落下チェック距離（下方向）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeGuard")
	float GroundDownCheckDistance = 5000.0f;

	UFUNCTION()
	void UpdateLevelUI();

public:
	void BeginPlay();

	virtual void SetupInputComponent() override;

protected:
	// キャラクター操作開始時（Possess）に呼ばれる
	virtual void OnPossess(APawn* InPawn) override;

	// ========== 入力アクションに対応する処理 ==========

	// キャラクター移動
	void Move(const FInputActionValue& Value);

	// カメラ操作
	void Look(const FInputActionValue& Value);

	// ダッシュ開始
	void RunStart();

	// ダッシュ終了
	void RunStop();

	// カメラズーム切替（3段階：遠→中→近）
	void HandleZoom();

	// インタラクト（調べる・拾う等）
	void HandleInteract();

	// カーソルを表示（L1押下時）
	void HandleCursorPressed();

	// カーソルを非表示（L1離した時）
	void HandleCursorReleased();

	// ズーム状態：0 = 遠距離 / 1 = 追従視点 / 2 = ZoomIn
	int32 ZoomLevel = 0;

	// ========== 落下防止(地面チェック） ==========

	/** 指定方向に地面があるかチェックして安全か判定 */
	bool IsSafeToMoveInDirection(const FVector& Direction);
};
