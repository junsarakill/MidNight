// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_FSM.h"

#include "JSH_FloatingTest.h"
#include "JSH_GameInstance.h"
#include "JSH_Player.h"
#include "JSH_Point.h"

// Sets default values for this component's properties
UJSH_FSM::UJSH_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSH_FSM::BeginPlay()
{
	Super::BeginPlay();

	// 시작 State
	TState = TopState::Start;

	Player = Cast<AJSH_Player>(GetOwner());

	GameInstance = Cast<UJSH_GameInstance>(GetWorld()->GetGameInstance());
}


// Called every frame
void UJSH_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString myState = UEnum::GetValueAsString(TState);
	DrawDebugString(GetWorld() , GetOwner()->GetActorLocation(), myState , nullptr , FColor::Yellow , 0, true, 1);
	
	// FSM
	switch (TState)
	{
	case TopState::Idle:
		IdleState();
		break;
		
	case TopState::Start:
		StartState(DeltaTime);
		break;

	case TopState::Point01:
		Point01State();
		break;

	case TopState::Point02:
		Point02State();
		break;

	case TopState::Point03:
		Point03State();
		break;
	}

	
	if (ClickName == "BP_Point01")
	{
		TState = TopState::Point01;
		ClickName = "Null";
	}

	if (ClickName == "BP_Point02")
	{
		TState = TopState::Point02;
		ClickName = "Null";
	}

	if (ClickName == "BP_Point03")
	{
		TState = TopState::Point03;
		ClickName = "Null";
	}
	

	// GEngine->AddOnScreenDebugMessage(8, 1, FColor::Blue, FString::Printf(TEXT("Tt")));
	//


	// GameInstance의 GameProgress에 따라 , Point 막아주기
	if (GameInstance->GameProgress == 0)
	{
	}
	else if(GameInstance->GameProgress == 1)
	{
		End01 = false;
	}
	else if(GameInstance->GameProgress == 2)
	{
		End01 = false;
		End02 = false;
	}
	else if(GameInstance->GameProgress == 3)
	{
		End01 = false;
		End02 = false;
		End03 = false;
	}
		
}

void UJSH_FSM::IdleState()
{
}

void UJSH_FSM::StartState(float DeltaTime)
{
	// 나중에는 StartWidget이랑 연결해서 startState로 넘어가도록 연결하기
	// if (startP)
	// {
	// 	FVector destination = StartPoint->GetActorLocation();
	// 	FVector dir = destination - Player->GetActorLocation();
	// 	Player->AddMovementInput(dir.GetSafeNormal());
	//
	// 	if (dir.Size() < ReachDistance)
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
	// 		TState = TopState::Idle;
	// 		startP = false;
	// 		StartText->startVisible = true;
	// 	}
	// }
	// else
	// {
	// 	TState = TopState::Idle;
	// }
}

void UJSH_FSM::Point01State()
{
	if (End01)
	{
		FVector destination = Point01->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			CoinFun();
			GameInstance->GameProgress = 1;
			GetCoin = true; 
			End01 = false;

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 1;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}

void UJSH_FSM::Point02State()
{
	if (End02)
	{
		FVector destination = Point02->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			CoinFun();
			GameInstance->GameProgress = 2;
			GetCoin = true; 
			End02 = false;

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 2;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}

void UJSH_FSM::Point03State()
{
	if (End03)
	{
		FVector destination = Point03->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			GameInstance->GameProgress = 3;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("GameProgress: %d"), GameInstance->GameProgress));
			OpenLevel03 = true; // false로는 bp에서 변경
			End03 = false;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}



void UJSH_FSM::GameinstanceUpdate() const
{
	// UJSH_GameInstance* GameInstance = Cast<UJSH_GameInstance>(GetWorld()->GetGameInstance());
	
	// GameInstance가 유효한지 확인
	if (GameInstance)
	{
		GameInstance->EndVector = PlayerEndVector;
	}
}

void UJSH_FSM::CoinFun()
{
	GetCoin = true;
	GameInstance->BookPoint += 1;
}



