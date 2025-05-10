// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectBase.h"
#include "IA_MoveTrigger.generated.h"

/**
 * 
 */
UCLASS()
class AIA_MoveTrigger : public AObjectBase
{
	GENERATED_BODY()
	
public:
	AIA_MoveTrigger();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract_Implementation() override;

	/** 延遲摧毀自身 */
	void DeferredDestroy();

public:
	/** 被移動的目標物件 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	AActor* TargetToMove = nullptr;

	/** 移動偏移量（例：向上 100 cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	FVector MoveOffset = FVector(0, 0, 100);

	/** 是否在互動後讓本體延遲消失 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	bool bDestroyAfterInteract = true;

	/** 緩慢移動速度（單位：cm/sec） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveTrigger")
	float MoveSpeed = 300.f;

private:
	FTimerHandle DestroyTimerHandle;
	bool bShouldMove = false;
	FVector MoveTargetLocation;
};

