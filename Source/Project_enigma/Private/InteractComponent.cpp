
#include "InteractComponent.h"
#include "ObjectBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// ================================
// 実際にインタラクト（調べる・拾う）を試みる処理
// ================================
void UInteractComponent::TryInteract()
{
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	// 射線開始點：角色の少し上（例：頭）
	FVector Start = Player->GetActorLocation() + FVector(0, 0, 50);

	// 射線結束點：足元の下方向（距離：InteractDistance）
	FVector End = Start + FVector(0, 0, -InteractDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AObjectBase* Target = Cast<AObjectBase>(Hit.GetActor());
		if (Target && Target->bIsPlayerInteractable)
		{
			Target->OnInteract();
		}
	}

	// デバッグ表示（下向きの緑線）
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 2.f);
}