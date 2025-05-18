// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EdgeGuardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEdgeGuardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UEdgeGuardComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** 一定以下に落ちたら復帰させるZ座標 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeGuard")
    float SafeZThreshold = -200.0f;

    /** プレイヤーが最後に立っていた安全な位置 */
    FVector LastSafeLocation;

    /** 地面がなければ進行を止める：前方距離 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeGuard|ForwardCheck")
    float ForwardCheckDistance = 50.0f;

    /** 地面がなければ進行を止める：下方距離 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeGuard|ForwardCheck")
    float DownCheckDistance = 150.0f;

    /** デバッグ表示 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeGuard|Debug")
    bool bDebugDraw = true;

    /** デバッグ線の表示時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EdgeGuard|Debug")
    float DebugDrawDuration = 1.0f;
};