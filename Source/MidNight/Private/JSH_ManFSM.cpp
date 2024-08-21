// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_ManFSM.h"

#include "JSH_Man.h"
#include "JSH_Player.h"

// Sets default values for this component's properties
UJSH_ManFSM::UJSH_ManFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSH_ManFSM::BeginPlay()
{
	Super::BeginPlay();

	MState = Manstate::Walk;

	Me = Cast<AJSH_Man>(GetOwner());

	Player = Cast<AJSH_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	// ...
}


// Called every frame
void UJSH_ManFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FString myState = UEnum::GetValueAsString(MState);
	DrawDebugString(GetWorld() , GetOwner()->GetActorLocation(), myState , nullptr , FColor::Blue , 0, true, 1);

	switch (MState)
	{
	case Manstate::Idle:
		IdleState();
		break;
		
	case Manstate::Walk:
		WalkState();
		break;

	case Manstate::Cage:
		CageState();
		break;
	}
}

void UJSH_ManFSM::IdleState()
{
}

void UJSH_ManFSM::WalkState()
{
	FVector destination = Player->GetActorLocation();
	FVector dir = destination - Me->GetActorLocation();
	Me->AddMovementInput(dir.GetSafeNormal());
	
	// if (dir.Size() < ReachDistance)
	// {
	// 	GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
	// 	MState = Manstate::Idle;
	// }
}

void UJSH_ManFSM::CageState()
{
}

