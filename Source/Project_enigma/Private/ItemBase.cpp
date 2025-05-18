#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "CharacterControllerBase.h"
#include "WBP_LevelUIBase.h"

AItemBase::AItemBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Root コンポーネント
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // メッシュ（見た目）
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    // 当たり判定の球（プレイヤーとぶつかったら反応）
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->InitSphereRadius(100.f);
    InteractionSphere->SetupAttachment(RootComponent);

    // メッシュはぶつからない
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 球はプレイヤーにだけ反応する
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AItemBase::BeginPlay()
{
    Super::BeginPlay();

    // 衝突ありにしたいときだけ有効化
    if (bEnableCollision)
    {
        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    // プレイヤーが触れたら自動で取得
    if (bCollectOnOverlap)
    {
        InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::HandleOverlap);
    }

    // 最初のサイズを記録（縮小用）
    OriginalScale = GetActorScale3D();
}

void AItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 回転アニメ（見た目）
    FRotator Spin(0.f, RotationSpeed * DeltaTime, 0.f);
    AddActorLocalRotation(Spin);

    // 縮小アニメ中ならサイズ変更
    if (bIsShrinking)
    {
        UpdateShrink(DeltaTime);
    }
}

// 他のアクター（プレイヤー）とぶつかったときの処理
void AItemBase::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        CollectItem(OtherActor);
    }
}

// 実際のアイテム収集処理
void AItemBase::CollectItem(AActor* Collector)
{
    OnCollected(Collector);
}

// アイテムが取られたときの処理
void AItemBase::OnCollected(AActor* Collector)
{
    if (bCollected) return;
    bCollected = true;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    ACharacterControllerBase* MyController = Cast<ACharacterControllerBase>(PC);

    // 効果音を再生
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }



    // UI を更新（ダイヤの場合）
    if (ItemType == EItemType::EIT_Crystal)
    {
        MyController->LevelUIInstance->SetDiamondCount(1);
    }
   
    if (ItemType == EItemType::EIT_Coin)
    {
        MyController->LevelUIInstance->SetCoinCount(1);
    }

    // ブループリントイベント通知（例：エフェクト再生）
    OnItemCollected.Broadcast();

    // 縮小アニメ開始 → 最後に Destroy()
    StartShrinkAndDestroy();
}

// 縮小開始
void AItemBase::StartShrinkAndDestroy()
{
    bIsShrinking = true;
    ShrinkElapsedTime = 0.f;
}

// 毎フレームの縮小処理
void AItemBase::UpdateShrink(float DeltaTime)
{
    ShrinkElapsedTime += DeltaTime;
    float Progress = FMath::Clamp(ShrinkElapsedTime / ShrinkDuration, 0.f, 1.f);

    // スケールを徐々に 0 に近づける
    FVector NewScale = FMath::Lerp(OriginalScale, FVector::ZeroVector, Progress);
    SetActorScale3D(NewScale);

    // 消えたら Destroy()
    if (Progress >= 1.f)
    {
        Destroy();
    }
}