// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_RotationHandComponent.h"

#include "MH_Scene04GameModeBase.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UMH_RotationHandComponent::UMH_RotationHandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMH_RotationHandComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	GM04 = Cast<AMH_Scene04GameModeBase>(GetWorld()->GetAuthGameMode());
}


// Called every frame
void UMH_RotationHandComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRotating && CurrentHandActor)
	{
		// 회전 중일 때 목표 회전으로 서서히 회전
		FRotator CurrentRotation = FMath::RInterpTo(CurrentHandActor->GetActorRotation(), TargetRotation, DeltaTime,
		                                            2.0f);
		CurrentHandActor->SetActorRotation(CurrentRotation);

		// 목표 회전에 도달하면 멈춤
		if (CurrentRotation.Equals(TargetRotation, 1.0f))
		{
			bIsRotating = false;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMH_RotationHandComponent::ResetRotation, 1.0f,
			                                       false);
		}
	}
}

void UMH_RotationHandComponent::RotateHand(int32 InputValue)
{
	FString TagName = (InputValue == 1) ? TEXT("left") : TEXT("right");
	CurrentHandActor = nullptr;

	// 월드에서 지정된 태그를 가진 액터를 찾음
	for (AActor* Actor : GetWorld()->GetCurrentLevel()->Actors)
	{
		if (Actor && Actor->GetName().Contains(TEXT("GretelHand")))
		{
			CurrentHandActor = Actor;
			break;
		}
	}

	if (CurrentHandActor)
	{
		InitialRotation = CurrentHandActor->GetActorRotation();
		TargetRotation = InitialRotation + FRotator(0, 0.f, -100.f); // Roll 축으로 -120도 회전
		bIsRotating = true;
	}
}

void UMH_RotationHandComponent::ResetRotation()
{
	if (CurrentHandActor)
	{
		TargetRotation = InitialRotation;
		bIsRotating = true;
		if (GM04)
		{
			GM04->Scene04 = true;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("asd"));
			GM04->CheckLevelTransition();
		}
	}
}
