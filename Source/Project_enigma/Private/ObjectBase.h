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
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bIsPlayerInteractable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bIsAutoExecute = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	EObjectType ObjectType = EObjectType::None;

	// === Interact Events ===
	UFUNCTION(BlueprintNativeEvent, Category = "Object")
	void OnInteract();
	virtual void OnInteract_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Object")
	void OnAutoExecute();
	virtual void OnAutoExecute_Implementation();

protected:
	// === 見た目・構造 ===
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Visual")
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	// === エフェクト・サウンド・アニメ ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	USoundBase* InteractSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	UParticleSystem* InteractEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	UAnimationAsset* InteractAnimation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object|Effect")
	FVector EffectOffset = FVector::ZeroVector;

	// === 自動滑らか回転 ===
	UPROPERTY(EditAnywhere, Category = "AutoRotation")
	FRotator RotationSpeed = FRotator(0.f, 90.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "AutoRotation")
	float AutoExecuteInterval = 2.0f;

	UPROPERTY(EditAnywhere, Category = "SmoothRotation")
	float RotationSpeedDegPerSec = 180.f;

private:
	FTimerHandle AutoExecuteTimerHandle;
	bool bIsRotating = false;
	FRotator TargetRotation;

	void StartSmoothRotation();
};