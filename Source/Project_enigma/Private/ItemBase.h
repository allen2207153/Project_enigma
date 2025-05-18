#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ItemBase.generated.h"

// 🔸Enum：アイテムの種類
UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_Crystal     UMETA(DisplayName = "Crystal"),
    EIT_Key         UMETA(DisplayName = "Key"),
    EIT_Coin        UMETA(DisplayName = "Coin"),
    EIT_Crown       UMETA(DisplayName = "Crown"),
    EIT_Trigger     UMETA(DisplayName = "Trigger")
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
    void StartShrinkAndDestroy();    // 小さくなって消える
    void UpdateShrink(float DeltaTime); // 毎フレームの縮小処理

public:
    // アイテムの見た目（Mesh）
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMeshComponent* Mesh;

    // アイテムを取る判定用の球
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    class USphereComponent* InteractionSphere;

    // アイテムの種類（ダイヤ／カギなど）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    // プレイヤーがぶつかったら自動で取るかどうか
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bCollectOnOverlap = true;

    // 物理衝突を有効にするか
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bEnableCollision = false;

    // アイテムを取った時に流す音
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
    USoundBase* PickupSound;

    // ゆっくり回るスピード（見た目）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Visual")
    float RotationSpeed = 180.f;

    // もう取られたかどうか
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    bool bCollected = false;

    // ブループリントイベント：取られた時に呼ばれる
    UPROPERTY(BlueprintAssignable, Category = "Item")
    FOnItemCollected OnItemCollected;

    // 🔸現在の収集数（UI 表示用）

    // ダイヤ（Crystal）の数
    UPROPERTY(BlueprintReadOnly, Category = "Item|UI")
    int32 CollectedCrystalCount = 0;

    // コイン（Coin）の数
    UPROPERTY(BlueprintReadOnly, Category = "Item|UI")
    int32 CollectedCoinCount = 0;

private:
    // 縮小処理用の変数
    bool bIsShrinking = false;
    float ShrinkDuration = 0.3f;
    float ShrinkElapsedTime = 0.f;
    FVector OriginalScale;
};