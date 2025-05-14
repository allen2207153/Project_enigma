#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ItemBase.generated.h"

// 🔸Enum：道具類型
UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_Crystal     UMETA(DisplayName = "Crystal"),
    EIT_Key         UMETA(DisplayName = "Key"),
    EIT_Heal        UMETA(DisplayName = "Heal"),
    EIT_Speed       UMETA(DisplayName = "Speed"),
    EIT_Trigger     UMETA(DisplayName = "Trigger")
};
// 🔸Delegate：道具被收集時可綁定的事件
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
    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable, Category = "Item")
    void CollectItem(AActor* Collector);

    virtual void OnCollected(AActor* Collector);

private:
    void StartShrinkAndDestroy();  // 收集動畫入口
    void UpdateShrink(float DeltaTime); // 每幀縮小

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    class USphereComponent* InteractionSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bCollectOnOverlap = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bEnableCollision = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
    USoundBase* PickupSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Visual")
    float RotationSpeed = 180.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    bool bCollected = false;

    UPROPERTY(BlueprintAssignable, Category = "Item")
    FOnItemCollected OnItemCollected;

private:
    bool bIsShrinking = false;
    float ShrinkDuration = 0.3f;         // 持續時間（秒）
    float ShrinkElapsedTime = 0.f;
    FVector OriginalScale;
};