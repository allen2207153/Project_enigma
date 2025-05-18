#include "EdgeGuardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

UEdgeGuardComponent::UEdgeGuardComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UEdgeGuardComponent::BeginPlay()
{
    Super::BeginPlay();

    ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
    if (OwnerChar)
    {
        LastSafeLocation = OwnerChar->GetActorLocation(); // 初期位置を安全地点とする
    }
}

void UEdgeGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
    if (!OwnerChar) return;

    FVector CurrentLocation = OwnerChar->GetActorLocation();

    // ✅ 前方足元チェック（地面がなければ移動を止める）
    FVector Forward = OwnerChar->GetActorForwardVector();
    FVector Start = CurrentLocation + Forward * ForwardCheckDistance;
    FVector End = Start - FVector(0, 0, DownCheckDistance);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerChar);

    bool bHasGround = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (!bHasGround)
    {
        // 地面がなければ移動を止める（崖に落ちそうな場合）
        OwnerChar->GetCharacterMovement()->StopMovementImmediately();

        if (bDebugDraw)
        {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, DebugDrawDuration);
        }

        return; // ここで終了（落下チェックはスキップ）
    }

    // ✅ 地面に立っていれば安全地点を記録
    if (OwnerChar->GetCharacterMovement()->IsMovingOnGround())
    {
        LastSafeLocation = CurrentLocation;

        if (bDebugDraw)
        {
            DrawDebugSphere(GetWorld(), LastSafeLocation, 20.0f, 12, FColor::Green, false, DebugDrawDuration);
        }
    }

    // ✅ 落下復帰（Z が閾値を下回ったら元の位置に戻す）
    if (CurrentLocation.Z < SafeZThreshold)
    {
        OwnerChar->SetActorLocation(LastSafeLocation);
        OwnerChar->GetCharacterMovement()->StopMovementImmediately();

        UE_LOG(LogTemp, Warning, TEXT("🛟 EdgeGuard: Respawn triggered"));
    }
}