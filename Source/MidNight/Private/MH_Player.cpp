// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_Player.h"
#include "BS_ServerManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMH_Player::AMH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMH_Player::BeginPlay()
{
	Super::BeginPlay();

	// @@ 게임 인스턴스 가져와서 해당 게임 진행도 값 설정
	
}

// Called every frame
void AMH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
// qte 시작
void AMH_Player::StartQTE()
{
	// 이미 qte 중이면 return
	if(onQTE)
		return;

	// 현재 장면에 맞는 qte 실행
	switch(gameIdx)
	{
		case 1:
			StartMotionCapture(EQTEType::THROW);
			break;
		case 2:
			StartMotionCapture(EQTEType::EATING);
			break;
		case 3:
			StartMotionCapture(EQTEType::CLEAN);
			break;
		case 4:
			StartMotionCapture(EQTEType::GIVE);
			break;
		case 5:
			StartMotionCapture(EQTEType::THROW);
			break;
	}
	
}
// 서버 매니저 찾아서 해당 qte 실행 요청
void AMH_Player::StartMotionCapture(EQTEType type)
{
	auto* sm = Cast<ABS_ServerManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ABS_ServerManager::StaticClass()));
	if(sm)
	{
		sm->player = this;
		//해당 액터로 뭔가 하기
		// 모션 캡처 실행
		sm->RunAsyncScript(type);
		// tcp 통신 실행
		sm->CreateClient(TEXT(""), 0);
	}
}

// 데이터 받기
void AMH_Player::ReceiveDataFromSM(int32 data)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("데이터 받음 : %d"), data));
	// @@ 행동 하게 작성
}
