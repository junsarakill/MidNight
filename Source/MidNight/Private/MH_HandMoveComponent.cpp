// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_HandMoveComponent.h"

#include "MH_Scene03GameMode.h"

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
	GM03= Cast<AMH_Scene03GameMode>(GetWorld()->GetAuthGameMode());
	
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
			if (GM03)
			{
				GM03->Scene03 = true;
			}
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

