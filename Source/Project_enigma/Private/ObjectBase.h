// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectBase.generated.h"


class USoundBase;
class UParticleSystem;
class UAnimationAsset;
class USkeletalMeshComponent;





UENUM(BlueprintType)

enum class EObjectType : uint8
{
	None            UMETA(DisplayName = "None"),
	Interactable    UMETA(DisplayName = "Interactable"),
	AutoRotating    UMETA(DisplayName = "Auto Rotating"),
};

UCLASS()
class AObjectBase : public AActor
{
	GENERATED_BODY()



public:
	AObjectBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** 玩家是否可以按鍵互動 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bIsPlayerInteractable = false;

	/** 是否自動執行（Tick中） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bIsAutoExecute = false;

	/** 類別分類 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	EObjectType ObjectType = EObjectType::None;

	/** 玩家互動時執行 */
	UFUNCTION(BlueprintNativeEvent, Category = "Object")
	void OnInteract();
	virtual void OnInteract_Implementation();

	/** 自動執行邏輯（每隔一段時間） */
	UFUNCTION(BlueprintNativeEvent, Category = "Object")
	void OnAutoExecute();
	virtual void OnAutoExecute_Implementation();

protected:
	/** 靜態網格（主體） */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	/** 可選：動畫播放用骨架網格 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	/** 自動旋轉角度（一次執行轉動量） */
	UPROPERTY(EditAnywhere, Category = "AutoRotation")
	FRotator RotationSpeed = FRotator(0.f, 90.f, 0.f);

	/** 自動執行間隔（秒） */
	UPROPERTY(EditAnywhere, Category = "AutoRotation")
	float AutoExecuteInterval = 2.0f;

	/** 音效（互動時播放） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	USoundBase* InteractSound = nullptr;

	/** 粒子特效（Cascade） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	UParticleSystem* InteractEffect = nullptr;

	/** 特效偏移位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	FVector EffectOffset = FVector::ZeroVector;

	/** 動畫（互動時播放） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	UAnimationAsset* InteractAnimation = nullptr;

private:
	FTimerHandle AutoExecuteTimerHandle;
};