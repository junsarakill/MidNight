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

	case TopState::BreadEat:
		BreadEatState();
		break;

	case TopState::Point02:
		Point02State();
		break;

	case TopState::Point03:
		Point03State();
		break;

	case TopState::Point04:
		Point04State();
		break;
	
	case TopState::Point05:
		Point05State();
		break;

	case TopState::Point06:
		Point06State();
		break;

	case TopState::Point08:
		Point08State();
		break;

	case TopState::Point09:
		Point09State();
		break;
	}

	
	if (ClickName == "BP_Point01")
	{
		TState = TopState::Point01;
		ClickName = "Null";
	}

	if (ClickName == "BP_BreadEat")
	{
		TState = TopState::BreadEat;
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
	if (ClickName == "BP_Point04")
	{
		TState = TopState::Point04;
		ClickName = "Null";
	}
	if (ClickName == "BP_Point05")
	{
		TState = TopState::Point05;
		ClickName = "Null";
	}
	if (ClickName == "BP_Point06")
	{
		TState = TopState::Point06;
		ClickName = "Null";
	}
	if (ClickName == "BP_Point08")
	{
		TState = TopState::Point08;
		ClickName = "Null";
	}
	if (ClickName == "BP_Point09")
	{
		TState = TopState::Point09;
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
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			
			// Coin
			CoinFun();
			GetCoin = true;
			
			GameInstance->GameProgress = 1;
			
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

void UJSH_FSM::BreadEatState()
{
	if (EndBreadEat)
	{
		FVector destination = BreadEat->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			CoinFun();
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			
			//GameInstance->GameProgress = 1;
			
			GetCoin = true; 
			EndBreadEat = false;
			OpenLevelBread = true;

			// 바로 레벨 이동이라 위젯이 없어용
			//위젯을 실행시키는 함수
			// widgetOn = true;
			// WidgetINt = 1;
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
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();

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
			//OpenLevel03 = true; // false로는 bp에서 변경
			End03 = false;
			OpenLevelSnackHome = true;

			// Coin
			CoinFun();
			GetCoin = true;

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 3;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}

void UJSH_FSM::Point04State()
{
	if (End04)
	{
		FVector destination = Point04->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			
			GameInstance->GameProgress = 4;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("GameProgress: %d"), GameInstance->GameProgress));
			//OpenLevel03 = true; // false로는 bp에서 변경
			End04 = false;
			
			// Coin
			CoinFun();
			GetCoin = true;

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 4;

			DeleteMan = true;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}

void UJSH_FSM::Point05State()
{
	if (End05)
	{
		FVector destination = Point05->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			
			GameInstance->GameProgress = 5;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("GameProgress: %d"), GameInstance->GameProgress));
			//OpenLevel03 = true; // false로는 bp에서 변경
			End05 = false;
			

			// Coin
			CoinFun();
			GetCoin = true;

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 5;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}

void UJSH_FSM::Point06State()
{
	if (End06)
	{
		FVector destination = Point06->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			
			GameInstance->GameProgress = 6;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("GameProgress: %d"), GameInstance->GameProgress));
			//OpenLevel03 = true; // false로는 bp에서 변경
			End06 = false;
			

			// Coin
			CoinFun();
			GetCoin = true;

			// 손 레벨 이동
			OpenLevelHand = true;

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 6;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}

void UJSH_FSM::Point08State()
{
	if (End08)
	{
		FVector destination = Point08->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			
			GameInstance->GameProgress = 8;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("GameProgress: %d"), GameInstance->GameProgress));
			//OpenLevel03 = true; // false로는 bp에서 변경
			End08 = false;
			

			// Coin
			CoinFun();
			GetCoin = true;
			

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 8;

			OpenLevelpor = true;
		}
	}
	else
	{
		TState = TopState::Idle;
	}
}

void UJSH_FSM::Point09State()
{
	if (End09)
	{
		FVector destination = Point09->GetActorLocation();
		FVector dir = destination - Player->GetActorLocation();
		Player->AddMovementInput(dir.GetSafeNormal());

		if (dir.Size() < ReachDistance)
		{
			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Yellow, FString::Printf(TEXT("yeee")));
			TState = TopState::Idle;
			
			PlayerEndVector = Player->GetActorLocation();
			GameinstanceUpdate();
			
			GameInstance->GameProgress = 9;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("GameProgress: %d"), GameInstance->GameProgress));
			//OpenLevel03 = true; // false로는 bp에서 변경
			End09 = false;
			

			// Coin
			CoinFun();
			GetCoin = true;
			

			//위젯을 실행시키는 함수
			widgetOn = true;
			WidgetINt = 9;
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



