// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
    virtual void BeginPlay() override;

public:
    /** 射線距離 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
    float InteractDistance = 200.f;

    /** 執行互動 */
    UFUNCTION(BlueprintCallable, Category = "Interact")
    void TryInteract();
};
