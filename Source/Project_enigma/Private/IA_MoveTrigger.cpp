#include "IA_MoveTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AIA_MoveTrigger::AIA_MoveTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	MoveOffset = FVector(0, 0, 150);
	MoveSpeed = 300.f;
	bDestroyAfterInteract = true;

	ObjectType = EObjectType::Interactable;
}

void AIA_MoveTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AIA_MoveTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldMove && TargetToMove)
	{
		FVector Current = TargetToMove->GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(Current, MoveTargetLocation, DeltaTime, MoveSpeed);
		TargetToMove->SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, MoveTargetLocation) < 1.f)
		{
			TargetToMove->SetActorLocation(MoveTargetLocation); // 精準補位
			bShouldMove = false;
		}
	}
}

void AIA_MoveTrigger::OnInteract_Implementation()
{
	Super::OnInteract_Implementation();

	// 隱藏自身 Mesh 並關閉碰撞
	if (Mesh)
	{
		Mesh->SetVisibility(false, true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 啟動目標移動
	if (TargetToMove)
	{
		MoveTargetLocation = TargetToMove->GetActorLocation() + MoveOffset;
		bShouldMove = true;
	}

	// 延遲摧毀
	if (bDestroyAfterInteract)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DestroyTimerHandle,
			this,
			&AIA_MoveTrigger::DeferredDestroy,
			0.5f,
			false
		);
	}
}

void AIA_MoveTrigger::DeferredDestroy()
{
	Destroy();
}