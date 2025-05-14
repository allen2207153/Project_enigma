#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/ActorComponent.h" 
#include "VirtualCursorComponent.generated.h"

class UUserWidget;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UVirtualCursorComponent : public UActorComponent{
	GENERATED_BODY()




public:
	UVirtualCursorComponent();


protected:
	virtual void BeginPlay() override;

	/** 現在の GameViewport サイズ（ピクセル単位）を取得 */
	FVector2D GetGameViewportSize() const;

	/** 現在の実行中の解像度（GSystemResolution から）を取得 */
	FVector2D GetGameResolution() const;

public:
	// 毎フレームの更新処理（カーソル位置の更新と射線方向の更新）
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** バーチャルカーソルを画面に表示・操作開始 */
	void EnableCursor();

	/** バーチャルカーソルを非表示・操作終了 */
	void DisableCursor();

	/** カーソル下にあるオブジェクトに対してインタラクト処理を実行（Aボタン対応） */
	void TryInteractWithCursor();

	/** 現在バーチャルカーソルが有効かどうか */
	UPROPERTY(BlueprintReadOnly, Category = "VirtualCursor")
	bool bCursorEnabled = false;

	/** 表示するカーソルウィジェットのクラス（UUserWidget を継承） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VirtualCursor")
	TSubclassOf<UUserWidget> VirtualCursorWidgetClass;

	/** 移動スピード（右スティック操作時の倍率） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VirtualCursor")
	float CursorSpeed = 800.f;

	/** UI・3Dオブジェクトにヒットする射線の最大距離 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VirtualCursor|Interaction")
	float InteractionDistance = 10000.f;




private:
	// ====================
	// 内部データ（状態管理）
	// ====================

	/** 実際に画面に表示されるカーソルウィジェットのインスタンス */
	UPROPERTY()
	UUserWidget* VirtualCursorWidget = nullptr;

	/** カーソルの現在位置（スクリーン座標） */
	FVector2D CursorPosition = FVector2D::ZeroVector;

	/** 現在の画面サイズ（Viewport サイズ） */
	FVector2D ViewportSize = FVector2D::ZeroVector;

	/** 右スティックのアナログ入力値 */
	FVector2D InputDirection = FVector2D::ZeroVector;

	/** 射線を飛ばすためのウィジェットインタラクションコンポーネント */
	UPROPERTY()
	UWidgetInteractionComponent* WidgetInteractor = nullptr;

	/** インタラクト時に使用するキー（Aボタン相当。左クリックなど） */
	FKey InteractKey = EKeys::LeftMouseButton;
};