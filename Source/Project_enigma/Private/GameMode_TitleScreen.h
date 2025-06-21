// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_TitleScreen.generated.h"

class UWBP_TitleScreen;


UCLASS()
class AGameMode_TitleScreen : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


	// 🔸Blueprintで設定できるカメラ参照
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<ACameraActor> DefaultCameraActor;


};
