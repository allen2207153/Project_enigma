// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraCenterActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
ACameraCenterActor::ACameraCenterActor()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    RootComponent = CameraBoom;
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->bUsePawnControlRotation = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    CenterLocation = FVector::ZeroVector; // 初期圓心設在原點（可自己設角色位置）
    CurrentYawAngle = 0.f;
    CurrentPitchAngle = -45.f; // 微微俯視
}

void ACameraCenterActor::BeginPlay()
{
    Super::BeginPlay();

    
    DefaultCenterLocation = CenterLocation;
    DefaultOrbitRadius = OrbitRadius;
}

void ACameraCenterActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShouldFollowTarget && FollowTarget)
    {
        CenterLocation = FollowTarget->GetActorLocation();
    }

    // 弧度変換（Yaw 正常，Pitch 反転！）
    float YawRad = FMath::DegreesToRadians(CurrentYawAngle);
    float PitchRad = FMath::DegreesToRadians(-CurrentPitchAngle);  // 反転

    // 球面座標 → 座標系
    float X = CenterLocation.X + OrbitRadius * FMath::Cos(PitchRad) * FMath::Cos(YawRad);
    float Y = CenterLocation.Y + OrbitRadius * FMath::Cos(PitchRad) * FMath::Sin(YawRad);
    float Z = CenterLocation.Z + OrbitRadius * FMath::Sin(PitchRad);

    FVector CameraPosition = FVector(X, Y, Z);
    SetActorLocation(CameraPosition);

    // 中心を向く
    FRotator LookAt = (CenterLocation - CameraPosition).Rotation();
    SetActorRotation(LookAt);

   
  
}

void ACameraCenterActor::TurnCamera(float Value)
{
    if (FMath::Abs(Value) > DeadZone)
    {
        CurrentYawAngle += Value;
    }
}

void ACameraCenterActor::LookUpCamera(float Value)
{
    if (FMath::Abs(Value) > DeadZone)
    {
        CurrentPitchAngle = FMath::Clamp(CurrentPitchAngle + Value, MinPitch, MaxPitch);
    }
}

void ACameraCenterActor::SetCenterLocation(const FVector& NewLocation)
{
    CenterLocation = NewLocation;
}

void ACameraCenterActor::SetOrbitRadius(float NewRadius)
{
    OrbitRadius = FMath::Max(50.f, NewRadius);
}

void ACameraCenterActor::SetFollowTarget(AActor* Target)
{
    FollowTarget = Target;
}

void ACameraCenterActor::SetShouldFollowTarget(bool bFollow)
{
    bShouldFollowTarget = bFollow;
}