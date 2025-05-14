#include "VirtualCursorComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "ObjectBase.h" 

UVirtualCursorComponent::UVirtualCursorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVirtualCursorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ✅ 自動綁定角色身上的 WidgetInteractor（如果沒指定）
	if (!WidgetInteractor)
	{
		WidgetInteractor = GetOwner()->FindComponentByClass<UWidgetInteractionComponent>();
		if (WidgetInteractor)
		{
			WidgetInteractor->bShowDebug = false;
			WidgetInteractor->bEnableHitTesting = true;
			WidgetInteractor->InteractionSource = EWidgetInteractionSource::World;
			UE_LOG(LogTemp, Warning, TEXT("🧩 WidgetInteractor found and bound."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ No WidgetInteractor found on owner."));
		}
	}
}

FVector2D UVirtualCursorComponent::GetGameViewportSize() const
{
	FVector2D Result = FVector2D(1, 1);

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize( /*out*/Result);
	}

	return Result;
}

FVector2D UVirtualCursorComponent::GetGameResolution() const
{
	FVector2D Result = FVector2D(1, 1);

	Result.X = GSystemResolution.ResX;
	Result.Y = GSystemResolution.ResY;

	return Result;
}

void UVirtualCursorComponent::EnableCursor()
{
	if (bCursorEnabled) return;
	bCursorEnabled = true;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return;

	ViewportSize = GetGameViewportSize();
	CursorPosition = ViewportSize * 0.5f;

	if (!VirtualCursorWidgetClass) return;

	VirtualCursorWidget = CreateWidget<UUserWidget>(PC, VirtualCursorWidgetClass);
	if (VirtualCursorWidget)
	{
		VirtualCursorWidget->AddToViewport();
		VirtualCursorWidget->SetAnchorsInViewport(FAnchors(0.f, 0.f));
		VirtualCursorWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		VirtualCursorWidget->SetPositionInViewport(CursorPosition, false);
	}
}

void UVirtualCursorComponent::DisableCursor()
{
	bCursorEnabled = false;
	if (VirtualCursorWidget)
	{
		VirtualCursorWidget->RemoveFromParent();
		VirtualCursorWidget = nullptr;
	}
}

void UVirtualCursorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bCursorEnabled) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;
	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return;

	float X, Y;
	PC->GetInputAnalogStickState(EControllerAnalogStick::CAS_RightStick, X, Y);
	InputDirection = FVector2D(X, Y);

	float DPIScale = UWidgetLayoutLibrary::GetViewportScale(this);
	ViewportSize = GetGameViewportSize();
	CursorPosition += InputDirection * CursorSpeed * DeltaTime;
	CursorPosition.X = FMath::Clamp(CursorPosition.X, 0.f, ViewportSize.X);
	CursorPosition.Y = FMath::Clamp(CursorPosition.Y, 0.f, ViewportSize.Y);

	if (VirtualCursorWidget)
	{
		VirtualCursorWidget->SetPositionInViewport(CursorPosition, false);
	}

	FVector2D TrueCursorPos = CursorPosition / DPIScale;
	GEngine->AddOnScreenDebugMessage(8, 0.f, FColor::Yellow,
		FString::Printf(TEXT("🖱️ CursorPosition: %.0f , %.0f (DPI scale %.2f)"), CursorPosition.X, CursorPosition.Y, DPIScale));
	GEngine->AddOnScreenDebugMessage(11, 0.f, FColor::White,
		FString::Printf(TEXT("📐 ViewportSize: %.0f x %.0f"), ViewportSize.X, ViewportSize.Y));

	if (WidgetInteractor)
	{
		FVector WorldOrigin, WorldDirection;
		if (PC->DeprojectScreenPositionToWorld(TrueCursorPos.X, TrueCursorPos.Y, WorldOrigin, WorldDirection))
		{
			WidgetInteractor->SetWorldLocation(WorldOrigin);
			WidgetInteractor->SetWorldRotation(WorldDirection.Rotation());
			WidgetInteractor->UpdateComponentToWorld();
			WidgetInteractor->TickComponent(DeltaTime, TickType, ThisTickFunction);

			DrawDebugLine(GetWorld(), WorldOrigin, WorldOrigin + WorldDirection * 1000.f, FColor::Blue, false, 0.f, 0, 1.f);
			DrawDebugPoint(GetWorld(), WorldOrigin, 10.f, FColor::Magenta, false, 0.1f);

			const FHitResult& Hit = WidgetInteractor->GetLastHitResult();
			if (Hit.bBlockingHit)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 8.f, 12, FColor::Green, false, 0.f);
				GEngine->AddOnScreenDebugMessage(5, 0.f, FColor::Green,
					FString::Printf(TEXT("✅ Interactor Hit: %s at %s"), *Hit.GetActor()->GetName(), *Hit.ImpactPoint.ToCompactString()));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(5, 0.f, FColor::Red, TEXT("❌ WidgetInteractor: No Hit"));
			}
		}
	}
}

void UVirtualCursorComponent::TryInteractWithCursor()
{
	if (!bCursorEnabled) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!OwnerCharacter || !PC) return;

	float DPIScale = UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D CorrectedCursorPos = CursorPosition / DPIScale;

	FVector WorldOrigin, WorldDirection;
	if (PC->DeprojectScreenPositionToWorld(CorrectedCursorPos.X, CorrectedCursorPos.Y, WorldOrigin, WorldDirection))
	{
		FVector TraceEnd = WorldOrigin + WorldDirection * 10000.f;
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwnerCharacter);

		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, WorldOrigin, TraceEnd, ECC_Visibility, Params);
		DrawDebugLine(GetWorld(), WorldOrigin, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 2.f, 0, 1.f);

		if (bHit && Hit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("🎯 命中：%s"), *Hit.GetActor()->GetName());

			if (Hit.GetActor()->ActorHasTag("Movable"))
			{
				Hit.GetActor()->AddActorWorldOffset(FVector(0, 0, 50));
				UE_LOG(LogTemp, Log, TEXT("🟢 移動成功：%s"), *Hit.GetActor()->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("❌ 未命中"));
		}
	}
}