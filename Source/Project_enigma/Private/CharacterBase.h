// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract, NotBlueprintable)
class ACharacterBase : public ACharacter
{
	GENERATED_BODY()


protected:
	//this is cameraArm_Date:20250425
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TWA_Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraArmComponent;

	//this is camera_Date:20250425
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TWA_Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
public:
	
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
