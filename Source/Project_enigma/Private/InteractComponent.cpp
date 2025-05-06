
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

void UInteractComponent::TryInteract()
{
    ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!Player) return;

    // 發射起點：角色中心位置（或 Mesh 的 Socket 也可以）
    FVector Start = Player->GetActorLocation() + FVector(0, 0, 50); // 微調高度避免貼地
    FVector Forward = Player->GetActorForwardVector();              // 面前方向
    FVector End = Start + Forward * InteractDistance;

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

    // Debug 線
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 2.f);
}
