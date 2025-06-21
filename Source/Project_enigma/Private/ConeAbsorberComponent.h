// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConeAbsorberComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UConeAbsorberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConeAbsorberComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 吸力範圍可視化用的靜態網格（Cone） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Absorb")
	UStaticMeshComponent* ConeTrigger;

	/** 吸引速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Absorb")
	float AbsorbSpeed = 300.f;

	/** 已進入範圍內等待吸取的物體 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Absorb")
	TArray<AActor*> AbsorbingActors;

	/** 初始化 ConeTrigger */
	void SetupCone(UStaticMeshComponent* InCone);

	/** 觸發進入 */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
