// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterControllerBase.generated.h"

class UInputMappingContext;
class UInputAction;
class ACharacterBase;
class ACameraCenterActor;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class ACharacterControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CurrentMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TWA_Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RunAction;

	TObjectPtr<ACharacterBase> CurrentCharacter;
	TObjectPtr<ACameraCenterActor> CameraCenterActor;

public:
	void BeginPlay();
	virtual void SetupInputComponent() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	/*void JumpStart();

	void JumpStop();*/

	void RunStart();

	void RunStop();

};
