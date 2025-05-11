#include "ObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimationAsset.h"
#include "TimerManager.h"
#include "Engine/World.h"

AObjectBase::AObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	ObjectType = EObjectType::None;
	RotationSpeed = FRotator(0.f, 90.f, 0.f);
	AutoExecuteInterval = 2.0f;
	RotationSpeedDegPerSec = 180.f;
}

void AObjectBase::BeginPlay()
{
	Super::BeginPlay();

	if (bIsAutoExecute && AutoExecuteInterval > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AutoExecuteTimerHandle,
			this,
			&AObjectBase::StartSmoothRotation,
			AutoExecuteInterval,
			true
		);
	}
}

void AObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotating)
	{
		FRotator Current = GetActorRotation();
		FRotator NewRotation = FMath::RInterpConstantTo(Current, TargetRotation, DeltaTime, RotationSpeedDegPerSec);

		SetActorRotation(NewRotation);

		if (NewRotation.Equals(TargetRotation, 0.1f))
		{
			SetActorRotation(TargetRotation);
			bIsRotating = false;
		}
	}
}

void AObjectBase::OnInteract_Implementation()
{
	// 再生：音
	if (InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
	}

	// 再生：パーティクル
	if (InteractEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InteractEffect, GetActorLocation() + EffectOffset);
	}

	// 再生：アニメーション（SkeletalMeshが設定されていれば）
	if (SkeletalMesh && InteractAnimation)
	{
		SkeletalMesh->PlayAnimation(InteractAnimation, false);
	}
}

void AObjectBase::OnAutoExecute_Implementation()
{
	
}

void AObjectBase::StartSmoothRotation()
{
	if (bIsRotating) return;

	TargetRotation = GetActorRotation() + RotationSpeed;
	bIsRotating = true;
}