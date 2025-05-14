
#include "CameraCenterActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACameraCenterActor::ACameraCenterActor()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    RootComponent = CameraBoom;
    CameraBoom->bDoCollisionTest = false;             // 壁との衝突を無視
    CameraBoom->bUsePawnControlRotation = false;      // プレイヤーの回転と同期しない

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;    // カメラは自分で回転制御

    // 初期値設定
    CenterLocation = FVector::ZeroVector;
    CurrentYawAngle = 0.f;
    CurrentPitchAngle = -45.f;
}

void ACameraCenterActor::BeginPlay()
{
    Super::BeginPlay();

    // デフォルト状態を保存（リセット時に使用可能）
    DefaultCenterLocation = CenterLocation;
    DefaultOrbitRadius = OrbitRadius;

    // 初期ターゲット値設定（カメラ移動の滑らか補間に使用）
    TargetCenterLocation = CenterLocation;
    TargetOrbitRadius = OrbitRadius;
}

void ACameraCenterActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ターゲット追従が有効かつターゲットが存在する場合、追従位置を更新
    if (bShouldFollowTarget && FollowTarget)
    {
        TargetCenterLocation = FollowTarget->GetActorLocation();
    }

    // 中心点を滑らかに補間移動
    CenterLocation = FMath::VInterpTo(CenterLocation, TargetCenterLocation, DeltaTime, FollowInterpSpeed);

    // カメラ距離（ズーム）も補間
    OrbitRadius = FMath::FInterpTo(OrbitRadius, TargetOrbitRadius, DeltaTime, FollowInterpSpeed);

    // 球面座標（角度＋半径）からカメラ位置を計算
    float YawRad = FMath::DegreesToRadians(CurrentYawAngle);
    float PitchRad = FMath::DegreesToRadians(-CurrentPitchAngle);

    float X = CenterLocation.X + OrbitRadius * FMath::Cos(PitchRad) * FMath::Cos(YawRad);
    float Y = CenterLocation.Y + OrbitRadius * FMath::Cos(PitchRad) * FMath::Sin(YawRad);
    float Z = CenterLocation.Z + OrbitRadius * FMath::Sin(PitchRad);

    FVector CameraPosition = FVector(X, Y, Z);
    SetActorLocation(CameraPosition);

    // 中心を向くように回転設定
    FRotator LookAt = (CenterLocation - CameraPosition).Rotation();
    SetActorRotation(LookAt);
}

// 水平方向（Yaw）を回転
void ACameraCenterActor::TurnCamera(float Value)
{
    if (FMath::Abs(Value) > DeadZone)
    {
        CurrentYawAngle += Value;
    }
}

// 垂直方向（Pitch）を回転（上限・下限あり）
void ACameraCenterActor::LookUpCamera(float Value)
{
    if (FMath::Abs(Value) > DeadZone)
    {
        CurrentPitchAngle = FMath::Clamp(CurrentPitchAngle + Value, MinPitch, MaxPitch);
    }
}

// カメラの中心点を即時変更
void ACameraCenterActor::SetCenterLocation(const FVector& NewLocation)
{
    CenterLocation = NewLocation;
    TargetCenterLocation = NewLocation;
}

// カメラの距離（ズーム）を即時変更
void ACameraCenterActor::SetOrbitRadius(float NewRadius)
{
    OrbitRadius = FMath::Max(50.f, NewRadius);  // 最小距離は 50.f
    TargetOrbitRadius = OrbitRadius;
}

// 追従ターゲットを設定
void ACameraCenterActor::SetFollowTarget(AActor* Target)
{
    FollowTarget = Target;
}

// 追従を有効／無効に設定
void ACameraCenterActor::SetShouldFollowTarget(bool bFollow)
{
    bShouldFollowTarget = bFollow;
}