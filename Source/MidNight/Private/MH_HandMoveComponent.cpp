// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_HandMoveComponent.h"

// Sets default values for this component's properties
UMH_HandMoveComponent::UMH_HandMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMH_HandMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	CurrentPosition = GetOwner()->GetActorLocation();
	
}


// Called every frame
void UMH_HandMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	if (bIsMoving)
	{
		FVector Direction = (TargetPosition - CurrentPosition).GetSafeNormal();
		CurrentPosition += Direction * MoveSpeed * DeltaTime;
		
		if (FVector::Dist(CurrentPosition, TargetPosition) < 1.0f)
		{
			CurrentPosition = TargetPosition;
			bIsMoving = false;
		}
		
		GetOwner()->SetActorLocation(CurrentPosition);
	}
}

void UMH_HandMoveComponent::MoveHandToTarget()
{
	FVector ForwardVector = GetOwner()->GetActorForwardVector(); // 손의 앞 방향 벡터
	TargetPosition = CurrentPosition + (ForwardVector * Distance);
	bIsMoving = true; // Start moving
}

