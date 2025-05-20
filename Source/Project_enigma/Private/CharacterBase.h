#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractComponent.h"
#include "VirtualCursorComponent.h"
//#include "EdgeGuardComponent.h"

#include "CharacterBase.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInteractComponent;
class UVirtualCursorComponent;
class UEdgeGuardComponent;




UCLASS(Abstract, NotBlueprintable)
class ACharacterBase : public ACharacter
{
	GENERATED_BODY()



protected:

	// ========== カメラ関連 ==========

	// カメラのアーム（キャラクターとの距離を調整）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TWA_Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraArmComponent;

	// 実際のカメラ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TWA_Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	// ========== インタラクト関連 ==========

	// インタラクト処理を担当するコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TWA_Interact", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractComponent> InteractComponent;

	// ========== バーチャルカーソル関連 ==========

	// バーチャルカーソル制御用のコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TWA_Cursor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVirtualCursorComponent> VirtualCursorComponent;

	////ステージ外への落下防止（エッジガード）
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TWA_EdgeGuard", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UEdgeGuardComponent> EdgeGuardComponent;

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 入力設定
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 外部からインタラクトコンポーネントを取得するための関数
	UInteractComponent* GetInteractComponent() const { return InteractComponent; }

	// コントローラー用にバーチャルカーソルコンポーネントを取得
	UVirtualCursorComponent* GetVirtualCursorComponent() const { return VirtualCursorComponent; }
};