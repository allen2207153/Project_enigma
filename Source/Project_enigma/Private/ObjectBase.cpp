// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectBase.h"

AObjectBase::AObjectBase()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
}

void AObjectBase::BeginPlay()
{
    Super::BeginPlay();
}

void AObjectBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsAutoExecute)
    {
        OnAutoExecute();
    }
}

void AObjectBase::OnInteract_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("ObjectBase::OnInteract() 被呼叫"));
}

void AObjectBase::OnAutoExecute_Implementation()
{
    AddActorLocalRotation(RotationSpeed * GetWorld()->GetDeltaSeconds());
}


