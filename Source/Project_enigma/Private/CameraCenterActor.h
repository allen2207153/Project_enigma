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

    // カメラを左右に回転させる（Yaw）
    void TurnCamera(float Value);

    // カメラを上下に回転させる（Pitch）
    void LookUpCamera(float Value);

protected:
    virtual void BeginPlay() override;

    // カメラの回転中心の位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
    FVector CenterLocation = FVector::ZeroVector;

    // カメラの回転半径（スプリングアームの長さ）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
    float OrbitRadius = 500.f;

    // スティック入力の無効範囲（小さい入力を無視）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
    float DeadZone = 0.1f;

    // カメラの最小仰角（上向き最大角度）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
    float MinPitch = -89.f;

    // カメラの最大仰角（下向き最大角度）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
    float MaxPitch = 0.f;

private:
    // カメラの支点（スプリングアーム）
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;

    // 実際に表示するカメラ
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> FollowCamera;

    // 現在の水平角度（Yaw）
    UPROPERTY(VisibleAnywhere)
    float CurrentYawAngle;

    // 現在の上下角度（Pitch）
    UPROPERTY(VisibleAnywhere)
    float CurrentPitchAngle;
};