#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "CharacterControllerBase.h"
#include "WBP_LevelUIBase.h"
#include "GameMode_LevelBase.h" // ✅ GameMode のヘッダーを追加

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

    OriginalScale = GetActorScale3D();
}

void AItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator Spin(0.f, RotationSpeed * DeltaTime, 0.f);
    AddActorLocalRotation(Spin);

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

    // 効果音を再生
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }

    // ✅ GameModeにアイテム取得を通知
    AGameMode_LevelBase* GM = Cast<AGameMode_LevelBase>(UGameplayStatics::GetGameMode(this));
    if (GM)
    {
        switch (ItemType)
        {
        case EItemType::E_Coin:
            GM->AddCoin();
            break;

        case EItemType::E_Crystal:
            GM->AddDiamond();
            break;

        case EItemType::E_Crown:
            GM->OnLevelClear();

        default:
            break;
        }
    }

    // ✅ ブループリント通知
    OnItemCollected.Broadcast();

    // ✅ 縮小アニメ開始
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