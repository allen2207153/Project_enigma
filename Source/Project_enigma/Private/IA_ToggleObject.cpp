#include "IA_ToggleObject.h"
#include "TimerManager.h"
#include "Engine/World.h"

AIA_ToggleObject::AIA_ToggleObject()
{
	PrimaryActorTick.bCanEverTick = false;
	bOnlyUsableFromWidget = true;
	bIsPlayerInteractable = true;
}

void AIA_ToggleObject::BeginPlay()
{
	Super::BeginPlay();
	OriginalTransform = GetActorTransform();
}

void AIA_ToggleObject::OnInteract_Implementation()
{
	// 立即中止前一段補間（如有）
	if (ToggleTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ToggleTimerHandle);
		bIsToggling = false;
	}

	const FTransform Start = GetActorTransform();
	const FTransform& End = bIsAtTarget ? OriginalTransform : TargetTransform;
	bIsToggling = true;

	float Elapsed = 0.f;
	GetWorld()->GetTimerManager().SetTimer(ToggleTimerHandle, FTimerDelegate::CreateLambda([this, Start, End, Elapsed]() mutable
		{
			Elapsed += GetWorld()->GetDeltaSeconds();
			float Alpha = FMath::Clamp(Elapsed / ToggleLerpTime, 0.f, 1.f);

			FTransform Blended;
			Blended.Blend(Start, End, Alpha);
			SetActorTransform(Blended);

			if (Alpha >= 1.f)
			{
				bIsAtTarget = !bIsAtTarget;
				bIsToggling = false;
				GetWorld()->GetTimerManager().ClearTimer(ToggleTimerHandle);
			}
		}), 0.01f, true);
}