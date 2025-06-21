// Fill out your copyright notice in the Description page of Project Settings.


#include "ConeAbsorberComponent.h"

UConeAbsorberComponent::UConeAbsorberComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UConeAbsorberComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ConeTrigger)
	{
		ConeTrigger->OnComponentBeginOverlap.AddDynamic(this, &UConeAbsorberComponent::OnOverlapBegin);
	}
}

void UConeAbsorberComponent::SetupCone(UStaticMeshComponent* InCone)
{
	ConeTrigger = InCone;

	if (ConeTrigger)
	{
		ConeTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ConeTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		ConeTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		ConeTrigger->OnComponentBeginOverlap.AddDynamic(this, &UConeAbsorberComponent::OnOverlapBegin);
	}
}

void UConeAbsorberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if (!Owner) return;

	for (int32 i = AbsorbingActors.Num() - 1; i >= 0; --i)
	{
		AActor* Target = AbsorbingActors[i];
		if (!IsValid(Target))
		{
			AbsorbingActors.RemoveAt(i);
			continue;
		}

		FVector Direction = (Owner->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
		FVector NewLocation = Target->GetActorLocation() + Direction * AbsorbSpeed * DeltaTime;
		Target->SetActorLocation(NewLocation);

		// 太靠近就銷毀
		if (FVector::DistSquared(NewLocation, Owner->GetActorLocation()) < 100.f)
		{
			Target->Destroy();
			AbsorbingActors.RemoveAt(i);
		}
	}
}

void UConeAbsorberComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner() && OtherActor->ActorHasTag("Absorbable"))
	{
		if (!AbsorbingActors.Contains(OtherActor))
		{
			AbsorbingActors.Add(OtherActor);
		}
	}
}