#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ItemBase.generated.h"

// 🔸Enum：アイテムの種類
UENUM(BlueprintType)
enum class EItemType : uint8
{
    E_Crystal     UMETA(DisplayName = "Crystal"),
    E_Key         UMETA(DisplayName = "Key"),
    E_Coin        UMETA(DisplayName = "Coin"),
    E_Crown       UMETA(DisplayName = "Crown"),
    E_Trigger     UMETA(DisplayName = "Trigger")
};
//アイテムを取ったときのイベント
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemCollected);



UCLASS()
class AItemBase : public AActor
{
    GENERATED_BODY()

public:
    AItemBase();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // プレイヤーとぶつかった時の処理
    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // プレイヤーがこのアイテムを取る
    UFUNCTION(BlueprintCallable, Category = "Item")
    void CollectItem(AActor* Collector);

    // アイテムが取られた時に呼ばれる処理（必要ならオーバーライドする）
    virtual void OnCollected(AActor* Collector);

private:
    void StartShrinkAndDestroy();         // 小さくなって消える
    void UpdateShrink(float DeltaTime);   // 毎フレームの縮小処理

public:
    // 🔸構成部品
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    class USphereComponent* InteractionSphere;

    // 🔸基本設定
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bCollectOnOverlap = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bEnableCollision = false;

    // 🔸視覚・効果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
    USoundBase* PickupSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Visual")
    float RotationSpeed = 180.f;

    // 🔸状態
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    bool bCollected = false;

    // 🔸ブループリント通知
    UPROPERTY(BlueprintAssignable, Category = "Item")
    FOnItemCollected OnItemCollected;

private:
    // 縮小処理用
    bool bIsShrinking = false;
    float ShrinkDuration = 0.3f;
    float ShrinkElapsedTime = 0.f;
    FVector OriginalScale;
};