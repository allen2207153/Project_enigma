// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPadComponent.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"
#include "GameFramework/Actor.h"
UJumpPadComponent::UJumpPadComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJumpPadComponent::BeginPlay()
{
	Super::BeginPlay();

	// 嘗試取得擁有者的碰撞元件（例如 Box 或 Mesh）
	if (AActor* Owner = GetOwner())
	{
		if (UPrimitiveComponent* CollisionComp = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
		{
			if (CollisionComp->GetGenerateOverlapEvents())
			{
				CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UJumpPadComponent::OnOverlapBegin);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Owner's root collision does not generate overlap events."));
			}
		}
	}
}

void UJumpPadComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MyCharacter = Cast<AMainCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->LaunchCharacter(BounceVelocity, true, true);
	}
}
