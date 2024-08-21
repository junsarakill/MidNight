// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_FSM.generated.h"



UENUM(BlueprintType)
enum class TopState : uint8
{
	Idle,
	Start,
	Point01,
	Point02,
	Point03,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIDNIGHT_API UJSH_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TopState TState = TopState::Start;

	// FSM 상태
	void IdleState();
	void StartState(float DeltaTime);
	void Point01State();
	void Point02State();
	void Point03State();

	// 한번만 실행되기 위한 Bool 값
	bool one = true;
	bool startP = true;
	bool End01 = true;
	bool End02 = true;
	bool End03 = true;

	// 플레이어(여자, 자신)
	UPROPERTY()
	class AJSH_Player* Player;

	// // 플레이어(남자)
	// UPROPERTY()
	// class AJSH_Man* Man;

	// 우클릭 시 저장되는 Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ClickName;


	// 이동 포인트 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Point* StartPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Point* Point01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Point* Point02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Point* Point03;


	UPROPERTY(EditAnywhere, Category=FSM)
	float ReachDistance = 150.0f;



	// 레벨 이동을 위한 함수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OpenLevel03 = false;


	// 레벨 이동 전 Player의 위치를 저장하는
	FVector PlayerEndVector;

	void GameinstanceUpdate() const;
	

	// Start Text
	float currtime = 0;
	float startime = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_FloatingTest* StartText;



	// Coin
	void CoinFun();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GetCoin = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// int BookPoint = 0; // GameInstacne로 옮김 변경


	// GameInstance
	class UJSH_GameInstance* GameInstance;
};
