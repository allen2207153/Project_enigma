
#include "InteractComponent.h"
#include "ObjectBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// ================================
// 実際にインタラクト（調べる・拾う）を試みる処理
// ================================
void UInteractComponent::TryInteract()
{
	// プレイヤーキャラクターを取得
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	// --------- レイキャスト設定 ---------
	// レイの発射位置（キャラの少し上あたり）
	FVector Start = Player->GetActorLocation() + FVector(0, 0, 50);

	// 向いている方向（カメラではなく、キャラクターの前方向）
	FVector Forward = Player->GetActorForwardVector();

	// レイの終点（インタラクト可能距離まで）
	FVector End = Start + Forward * InteractDistance;

	// ヒット情報
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player); // 自分自身は無視

	// --------- 可視チャネルでレイキャスト ---------
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		// インタラクト対象（ObjectBase派生かどうかチェック）
		AObjectBase* Target = Cast<AObjectBase>(Hit.GetActor());

		// インタラクト可能フラグがONなら実行
		if (Target && Target->bIsPlayerInteractable)
		{
			Target->OnInteract();
		}
	}

	// --------- デバッグ表示（緑線） ---------
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 2.f);
}