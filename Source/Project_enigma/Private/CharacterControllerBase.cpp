// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CharacterBase.h"
#include "CameraCenterActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode_LevelBase.h"
#include "WBP_LevelUIBase.h"


void ACharacterControllerBase::BeginPlay()
{
	Super::BeginPlay();

	SetViewTarget(CameraCenterActor);

	// ✅ 直接同步建立 UI，不用 AsyncTask
	if (LevelUIClass)
	{
		LevelUIInstance = CreateWidget<UWBP_LevelUIBase>(this, LevelUIClass);
		if (LevelUIInstance)
		{
			LevelUIInstance->AddToViewport();
			LevelUIInstance->SetDiamondCount(0);
			LevelUIInstance->SetCoinCount(0);
			LevelUIInstance->SetTime(0);
			UE_LOG(LogTemp, Warning, TEXT("🟢 UI Created Successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ UI creation failed"));
		}
	}

	// ✅ 開始每秒更新 UI
	GetWorldTimerManager().SetTimer(
		TimerHandle_UpdateTime,
		this,
		&ACharacterControllerBase::UpdateLevelUI,
		1.0f,
		true
	);
}

void ACharacterControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent
		= Cast<UEnhancedInputComponent>(this->InputComponent);

	if (EnhancedInputComponent)
	{
		// 移動入力
		EnhancedInputComponent->BindAction(MoveAction.Get(),
			ETriggerEvent::Triggered,
			this, &ACharacterControllerBase::Move);

		// 視点操作
		EnhancedInputComponent->BindAction(LookAction.Get(),
			ETriggerEvent::Triggered,
			this, &ACharacterControllerBase::Look);

		// ダッシュ開始
		EnhancedInputComponent->BindAction(RunAction.Get(),
			ETriggerEvent::Started,
			this, &ACharacterControllerBase::RunStart);

		// ダッシュ終了
		EnhancedInputComponent->BindAction(RunAction.Get(),
			ETriggerEvent::Completed,
			this, &ACharacterControllerBase::RunStop);

		// ズーム切り替え
		EnhancedInputComponent->BindAction(ZoomAction.Get(),
			ETriggerEvent::Started,
			this, &ACharacterControllerBase::HandleZoom);

		// インタラクト実行（ボタンでオブジェクトを操作）
		EnhancedInputComponent->BindAction(InteractAction.Get(),
			ETriggerEvent::Started,
			this, &ACharacterControllerBase::HandleInteract);

		// バーチャルカーソル有効化
		EnhancedInputComponent->BindAction(ToggleCursorAction.Get(),
			ETriggerEvent::Started,
			this, &ACharacterControllerBase::HandleCursorPressed);

		// バーチャルカーソル無効化
		EnhancedInputComponent->BindAction(ToggleCursorAction.Get(),
			ETriggerEvent::Completed,
			this, &ACharacterControllerBase::HandleCursorReleased);
	}
}

// ========== キャラクターを操作する際の初期化 ==========
void ACharacterControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 自身が操作するキャラクター取得
	this->CurrentCharacter = Cast<ACharacterBase>(InPawn);

	// カメラアクターを検索（レベル内にある最初のものを使用）
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraCenterActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		CameraCenterActor = Cast<ACameraCenterActor>(FoundActors[0]);
	}

	// 入力マッピングを登録（Enhanced Input）
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputLocalPlayerSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	InputLocalPlayerSubsystem->AddMappingContext(this->CurrentMappingContext.Get(), 0);
}

// ========== 移動処理（前後・左右） ==========
void ACharacterControllerBase::Move(const FInputActionValue& Value)
{
	const FVector2d MovementVector = Value.Get<FVector2D>();
	if (!CurrentCharacter || MovementVector.IsNearlyZero()) return;

	// カメラの Yaw に合わせて移動方向を計算
	const FRotator CameraRotation = CameraCenterActor->GetActorRotation();
	const FRotator YawOnlyRotation(0.f, CameraRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::Y);

	// ✅ 前方に地面があるかチェック（崖ガード）
	if (!IsSafeToMoveInDirection(ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X))
	{
		// UE_LOG(LogTemp, Warning, TEXT("⛔ 足元に地面なし。進行ブロック"));
		return;
	}

	// 入力ベクトルに応じてキャラクターを移動
	CurrentCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	CurrentCharacter->AddMovementInput(RightDirection, MovementVector.X);

	// 向きは固定
	FRotator CurrentRotation = CurrentCharacter->GetActorRotation();
	CurrentCharacter->SetActorRotation(CurrentRotation);
}

// ========== カメラ操作==========
void ACharacterControllerBase::Look(const FInputActionValue& Value)
{
	// カーソル有効中はカメラ操作しない
	if (CurrentCharacter->GetVirtualCursorComponent()->bCursorEnabled)
	{
		return;
	}

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (CameraCenterActor)
	{
		CameraCenterActor->TurnCamera(LookAxisVector.X);
		CameraCenterActor->LookUpCamera(LookAxisVector.Y);
	}
}

// ========== ダッシュ開始==========
void ACharacterControllerBase::RunStart()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
}

// ========== ダッシュ終了 ==========
void ACharacterControllerBase::RunStop()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed /= 2.0f;
}

// ========== カメラのズーム切り替え（3段階） ==========
void ACharacterControllerBase::HandleZoom()
{
	if (!CameraCenterActor || !CurrentCharacter) return;

	// ズームレベル 0→1→2→0 に循環
	ZoomLevel = (ZoomLevel + 1) % 3;

	// デバッグ表示
	FString Msg = FString::Printf(TEXT("ZoomLevel = %d"), ZoomLevel);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Msg);

	switch (ZoomLevel)
	{
	case 0: // 遠距離・固定視点
		CameraCenterActor->SetShouldFollowTarget(false);
		CameraCenterActor->SetTargetCenterLocation(CameraCenterActor->GetDefaultCenterLocation());
		CameraCenterActor->SetTargetOrbitRadius(CameraCenterActor->GetDefaultOrbitRadius());
		break;

	case 1: // 追従・中距離視点
		CameraCenterActor->SetFollowTarget(CurrentCharacter);
		CameraCenterActor->SetShouldFollowTarget(true);
		CameraCenterActor->SetTargetOrbitRadius(350.f);
		break;

	case 2: // 追従・近距離
		CameraCenterActor->SetTargetOrbitRadius(150.f);
		break;
	}
}

// ========== インタラクト入力：目の前のオブジェクトを調べる・拾うなど ==========
void ACharacterControllerBase::HandleInteract()
{
	auto* CursorComponent = CurrentCharacter->GetVirtualCursorComponent();
	if (CursorComponent->bCursorEnabled)
	{
		CursorComponent->TryInteractWithCursor();
	}
	else
	{
		CurrentCharacter->GetInteractComponent()->TryInteract();
	}
}

// ========== カーソルを有効化（ボタン押し） ==========
void ACharacterControllerBase::HandleCursorPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("L1 pressed: HandleCursorPressed() called"));

	if (!CurrentCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ CurrentCharacter is NULL"));
		return;
	}

	auto* CursorComponent = CurrentCharacter->GetVirtualCursorComponent();
	if (!CursorComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ VirtualCursorComponent is NULL"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("✅ Calling EnableCursor()"));
	CursorComponent->EnableCursor();
}

// ========== カーソルを無効化（ボタン離す） ==========
void ACharacterControllerBase::HandleCursorReleased()
{
	if (!CurrentCharacter) return;

	auto* CursorComponent = CurrentCharacter->GetVirtualCursorComponent();
	if (CursorComponent)
	{
		CursorComponent->DisableCursor();
	}
}

void ACharacterControllerBase::UpdateLevelUI()
{
	UE_LOG(LogTemp, Warning, TEXT("🟢 UpdateLevelUI() called"));

	auto* GM = Cast<AGameMode_LevelBase>(UGameplayStatics::GetGameMode(this));
	if (LevelUIInstance && GM)
	{
		LevelUIInstance->SetTime(GM->GetElapsedTime());
		LevelUIInstance->SetCoinCount(GM->GetCoins());
		LevelUIInstance->SetDiamondCount(GM->GetDiamonds());
	}
	else
	{
		if (!LevelUIInstance)
			UE_LOG(LogTemp, Error, TEXT("❌ LevelUIInstance is NULL"));
		if (!GM)
			UE_LOG(LogTemp, Error, TEXT("❌ GameMode is NULL"));
	}
}

bool ACharacterControllerBase::IsSafeToMoveInDirection(const FVector& Direction)
{
	if (!CurrentCharacter) return true;

	
	const FVector Start = CurrentCharacter->GetActorLocation() + Direction.GetSafeNormal() * GroundForwardCheckDistance;
	const FVector End = Start - FVector(0, 0, GroundDownCheckDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CurrentCharacter);

	bool bHitGround = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), Start, End, bHitGround ? FColor::Green : FColor::Red, false, 0.2f);
#endif

	return bHitGround;
}