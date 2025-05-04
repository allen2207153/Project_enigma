// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CharacterBase.h"
#include "CameraCenterActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ACharacterControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (CameraCenterActor)
	{
		SetViewTarget(CameraCenterActor);
	}
}
void ACharacterControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent
			= Cast<UEnhancedInputComponent>(this->InputComponent);

	if (EnhancedInputComponent)
	{
		//Move
		EnhancedInputComponent->BindAction(MoveAction.Get(),
														ETriggerEvent::Triggered, 
														this, &ACharacterControllerBase::Move);
		//Look
		EnhancedInputComponent->BindAction(LookAction.Get(),
														ETriggerEvent::Triggered,
														this, &ACharacterControllerBase::Look);
		//Run
		EnhancedInputComponent->BindAction(RunAction.Get(),
														ETriggerEvent::Started,
														this, &ACharacterControllerBase::RunStart);
		EnhancedInputComponent->BindAction(RunAction.Get(),
														ETriggerEvent::Completed,
														this, &ACharacterControllerBase::RunStop);
		//ZoomIn
		EnhancedInputComponent->BindAction(ZoomAction.Get(),
														ETriggerEvent::Started,
														this, &ACharacterControllerBase::HandleZoom);

	}
}

void ACharacterControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	this->CurrentCharacter = Cast<ACharacterBase>(InPawn);

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraCenterActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		CameraCenterActor = Cast<ACameraCenterActor>(FoundActors[0]);
	}

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputLocalPlayerSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	if (InputLocalPlayerSubsystem)
	{
		InputLocalPlayerSubsystem->AddMappingContext(this->CurrentMappingContext.Get(), 0);
	}
}

void ACharacterControllerBase::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2D>();

	const FRotator CameraRotation = CameraCenterActor->GetActorRotation();
	const FRotator YawOnlyRotation(0.f, CameraRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::Y);

	CurrentCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	CurrentCharacter->AddMovementInput(RightDirection, MovementVector.X);


	FRotator CurrentRotation = CurrentCharacter->GetActorRotation();
	CurrentCharacter->SetActorRotation(CurrentRotation);
	
	
}

void ACharacterControllerBase::Look(const FInputActionValue& Value)
{

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (CameraCenterActor)
	{
		CameraCenterActor->TurnCamera(LookAxisVector.X);
		CameraCenterActor->LookUpCamera(LookAxisVector.Y);
	}
}

void ACharacterControllerBase::RunStart()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
}

void ACharacterControllerBase::RunStop()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed /= 2.0f;

}

void ACharacterControllerBase::HandleZoom()
{
	if (!CameraCenterActor || !CurrentCharacter) return;

	ZoomLevel = (ZoomLevel + 1) % 3;

	FString Msg = FString::Printf(TEXT("ZoomLevel = %d"), ZoomLevel);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Msg);
	switch (ZoomLevel)
	{
	case 0:
		CameraCenterActor->SetShouldFollowTarget(false);
		CameraCenterActor->SetCenterLocation(CameraCenterActor->GetDefaultCenterLocation());
		CameraCenterActor->SetOrbitRadius(CameraCenterActor->GetDefaultOrbitRadius());
		break;

	case 1:
		CameraCenterActor->SetFollowTarget(CurrentCharacter); 
		CameraCenterActor->SetShouldFollowTarget(true);
		CameraCenterActor->SetCenterLocation(CurrentCharacter->GetActorLocation());
		CameraCenterActor->SetOrbitRadius(350.f);
		break;

	case 2:
		CameraCenterActor->SetFollowTarget(CurrentCharacter);
		CameraCenterActor->SetShouldFollowTarget(true);
		CameraCenterActor->SetOrbitRadius(150.f);
		break;
	}
}
