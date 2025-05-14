// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AItemBase::AItemBase()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->InitSphereRadius(100.f);
    InteractionSphere->SetupAttachment(RootComponent);

    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AItemBase::BeginPlay()
{
    Super::BeginPlay();

    if (bEnableCollision)
    {
        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    if (bCollectOnOverlap)
    {
        InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::HandleOverlap);
    }

    OriginalScale = GetActorScale3D(); // ✅ 紀錄原始大小
}

void AItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ✅ 道具旋轉（Idle動畫）
    FRotator Spin(0.f, RotationSpeed * DeltaTime, 0.f);
    AddActorLocalRotation(Spin);

    // ✅ 正在縮小動畫中
    if (bIsShrinking)
    {
        UpdateShrink(DeltaTime);
    }
}

void AItemBase::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        CollectItem(OtherActor);
    }
}

void AItemBase::CollectItem(AActor* Collector)
{
    OnCollected(Collector);
}

void AItemBase::OnCollected(AActor* Collector)
{
    if (bCollected) return;
    bCollected = true;

    // ✅ 播放音效
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }

    // ✅ 觸發 UI 或其他外部事件
    OnItemCollected.Broadcast();

    // ✅ 執行縮小動畫並延遲銷毀
    StartShrinkAndDestroy();
}

void AItemBase::StartShrinkAndDestroy()
{
    bIsShrinking = true;
    ShrinkElapsedTime = 0.f;
}

void AItemBase::UpdateShrink(float DeltaTime)
{
    ShrinkElapsedTime += DeltaTime;
    float Progress = FMath::Clamp(ShrinkElapsedTime / ShrinkDuration, 0.f, 1.f);

    FVector NewScale = FMath::Lerp(OriginalScale, FVector::ZeroVector, Progress);
    SetActorScale3D(NewScale);

    if (Progress >= 1.f)
    {
        Destroy();
    }
}