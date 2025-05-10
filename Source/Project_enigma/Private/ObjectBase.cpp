#include "ObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimationAsset.h"
#include "Engine/World.h"

AObjectBase::AObjectBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AObjectBase::BeginPlay()
{
	Super::BeginPlay();

	if (bIsAutoExecute && AutoExecuteInterval > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AutoExecuteTimerHandle,
			this,
			&AObjectBase::OnAutoExecute,
			AutoExecuteInterval,
			true
		);
	}
}

void AObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 已使用計時器，Tick 不需要旋轉
}

void AObjectBase::OnInteract_Implementation()
{
	// 播音效
	if (InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
	}

	// 播特效
	if (InteractEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InteractEffect, GetActorLocation() + EffectOffset);
	}

	// 播動畫（若有設定 SkeletalMesh）
	if (SkeletalMesh && InteractAnimation)
	{
		SkeletalMesh->PlayAnimation(InteractAnimation, false);
	}
}

void AObjectBase::OnAutoExecute_Implementation()
{
	AddActorLocalRotation(RotationSpeed);
}