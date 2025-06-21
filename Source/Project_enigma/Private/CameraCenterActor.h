// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraCenterActor.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACameraCenterActor : public AActor
{
    GENERATED_BODY()

public:
    ACameraCenterActor();
    virtual void Tick(float DeltaTime) override;

    // カメラを左右に回転
    void TurnCamera(float Value);

    // カメラを上下に回転（制限あり）
    void LookUpCamera(float Value);

    // カメラ回転の中心位置を即時設定
    void SetCenterLocation(const FVector& NewLocation);

    // カメラ回転の半径（ズーム距離）を即時設定
    void SetOrbitRadius(float NewRadius);

    // Blueprint から呼べる：目標の半径を設定（滑らかズーム用）
    UFUNCTION(BlueprintCallable, Category = "Camera|Orbit")
    void SetTargetOrbitRadius(float Radius) { TargetOrbitRadius = Radius; }

    // Blueprint から呼べる：目標の中心位置を設定（滑らか移動用）
    UFUNCTION(BlueprintCallable, Category = "Camera|Orbit")
    void SetTargetCenterLocation(const FVector& Location) { TargetCenterLocation = Location; }

    // 初期中心位置を取得（リセット用など）
    FVector GetDefaultCenterLocation() const { return DefaultCenterLocation; }

    // 初期ズーム距離を取得
    float GetDefaultOrbitRadius() const { return DefaultOrbitRadius; }

    // カメラ追従対象を設定
    void SetFollowTarget(AActor* Target);

    // カメラ追従を有効／無効に設定
    void SetShouldFollowTarget(bool bFollow);

protected:
    virtual void BeginPlay() override;

    // ========== 初期設定パラメータ（Blueprintから指定可能） ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Initial")
    FVector InitialCenterLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Initial")
    float InitialYaw = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Initial")
    float InitialPitch = -45.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Initial")
    float InitialOrbitRadius = 500.f;

    // ==== カメラ回転の基準座標・距離 ====

    // 現在のカメラ中心座標
    UPROPERTY(EditAnywhere, Category = "Camera|Orbit")
    FVector CenterLocation = FVector::ZeroVector;

    // 滑らか移動のための目標中心座標
    UPROPERTY(VisibleAnywhere, Category = "Camera|Orbit")
    FVector TargetCenterLocation = FVector::ZeroVector;

    // 現在のズーム距離（半径）
    UPROPERTY(EditAnywhere, Category = "Camera|Orbit")
    float OrbitRadius = 500.f;

    // 滑らかズームのための目標ズーム距離
    UPROPERTY(VisibleAnywhere, Category = "Camera|Orbit")
    float TargetOrbitRadius = 500.f;

    // 中心位置・半径両方に使う補間スピード（大きいほど速い）
    UPROPERTY(EditAnywhere, Category = "Camera|Smooth")
    float FollowInterpSpeed = 5.f;

    // 入力感度のデッドゾーン（小さい入力は無視）
    UPROPERTY(EditAnywhere, Category = "Camera|Orbit")
    float DeadZone = 0.1f;

    // カメラの上下制限（Pitch）
    UPROPERTY(EditAnywhere, Category = "Camera|Orbit")
    float MinPitch = -89.f;

    UPROPERTY(EditAnywhere, Category = "Camera|Orbit")
    float MaxPitch = 0.f;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;

    // 実際のカメラ
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> FollowCamera;

    // 現在の回転角度（Yaw, Pitch）
    UPROPERTY(VisibleAnywhere)
    float CurrentYawAngle;

    UPROPERTY(VisibleAnywhere)
    float CurrentPitchAngle;

    // デフォルトの状態（リセットなどに使用）
    UPROPERTY()
    FVector DefaultCenterLocation;

    UPROPERTY()
    float DefaultOrbitRadius;

    // カメラが追従するアクター
    UPROPERTY()
    AActor* FollowTarget = nullptr;

    // カメラ追従フラグ
    bool bShouldFollowTarget = false;
};