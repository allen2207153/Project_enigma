// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectBase.generated.h"

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
    bool bIsPlayerInteractable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
    bool bIsAutoExecute = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
    EObjectType ObjectType = EObjectType::None;

    UFUNCTION(BlueprintNativeEvent, Category = "Object")
    void OnInteract();
    virtual void OnInteract_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Object")
    void OnAutoExecute();
    virtual void OnAutoExecute_Implementation();

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, Category = "AutoRotation")
    FRotator RotationSpeed = FRotator(0.f, 90.f, 0.f);  // 每秒 Y 軸旋轉 90 度
};

