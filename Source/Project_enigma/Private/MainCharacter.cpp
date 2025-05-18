// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// ===========================
// プレイヤーキャラクターの初期設定
// ===========================
AMainCharacter::AMainCharacter()
{
	// プレイヤーの入力で直接回転しないようにする（カメラ制御専用）
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationRoll = false;

	// キャラクターの移動方向に自動で向きを変える設定
	TObjectPtr<UCharacterMovementComponent>  MovementComponent = this->GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;

	// ========== カメラアームの作成 ==========
	this->CameraArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("TWA SpringArmComponent"));
	this->CameraArmComponent->SetupAttachment(this->RootComponent.Get());
	this->CameraArmComponent->TargetArmLength = 400.f;              // アームの長さ（カメラ距離）
	this->CameraArmComponent->bUsePawnControlRotation = true;       // コントローラーの回転でアームを制御

	// ========== カメラ本体の作成 ==========
	this->CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TWA CameraComponent"));
	this->CameraComponent->SetupAttachment(this->CameraArmComponent.Get(), USpringArmComponent::SocketName);
	this->CameraComponent->bUsePawnControlRotation = false;         // カメラ自体は回転しない（アームに任せる）

	// ========== インタラクト処理用コンポーネント ==========
	this->InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("TWA InteractComponent"));

	// ========== バーチャルカーソル制御用コンポーネント ==========
	this->VirtualCursorComponent = CreateDefaultSubobject<UVirtualCursorComponent>(TEXT("TWA VirtualCursorComponent"));

	// ========== エッジガード制御用コンポーネント ==========
	//this->EdgeGuardComponent = CreateDefaultSubobject<UEdgeGuardComponent>(TEXT("TWA EdgeGuardComponent"));
}
