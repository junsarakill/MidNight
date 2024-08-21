// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_ManFSM.h"

#include "JSH_GameInstance.h"
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

	MState = Manstate::Idle;

	Me = Cast<AJSH_Man>(GetOwner());

	Player = Cast<AJSH_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// GameInstance = Cast<UJSH_GameInstance>(GetWorld()->GetGameInstance());
	
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

	// Cage에 갇히기 전 까지 돌다가, Cage 갇히면 false로 바꿔줘야 함
	if (Cage)
	{
		// 메인 플레이어와 너무 멀어졌을때 (ex: Map 이동 후) 옆으로 텔레포트 되도록 
		FVector destination = Player->GetActorLocation();
		FVector dir = destination - Me->GetActorLocation();
		float distance = dir.Size();
	
		if (distance >= 600.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Teleported to Player"));
			FVector teleportLocation = destination - dir.GetSafeNormal() * 200.0f;
			Me->SetActorLocation(teleportLocation);
			MState = Manstate::Idle;
			return;
		}

		// 바라보는 방향이 항상 Main 캐릭터 이도록
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(dir).Rotator();
		Me->SetActorRotation(LookAtRotation);
	}
}

void UJSH_ManFSM::IdleState()
{
	FVector destination = Player->GetActorLocation();
	FVector dir = destination - Me->GetActorLocation();
	
	if (dir.Size() >= IdleDistance)
	{
		GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("Stop")));
		MState = Manstate::Walk;
	}
}

void UJSH_ManFSM::WalkState()
{
	FVector destination = Player->GetActorLocation();
	FVector dir = destination - Me->GetActorLocation();
	Me->AddMovementInput(dir.GetSafeNormal());
	
	if (dir.Size() <= ReachDistance)
	{
		GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("Stop")));
		MState = Manstate::Idle;
	}
}

void UJSH_ManFSM::CageState()
{
}


