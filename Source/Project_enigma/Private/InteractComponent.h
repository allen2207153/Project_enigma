// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// コンストラクタ：デフォルト値の設定など
	UInteractComponent();

protected:
	virtual void BeginPlay() override;

public:
	/* インタラクト可能な最大距離*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float InteractDistance = 200.f;

	/* インタラクト実行処理 */
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void TryInteract();
};