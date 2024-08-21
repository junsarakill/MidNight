// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_ManFSM.generated.h"


UENUM(BlueprintType)
enum class Manstate : uint8
{
	Idle,
	Walk,
	Cage,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIDNIGHT_API UJSH_ManFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_ManFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	Manstate MState = Manstate::Walk;

	// FSM 상태
	void IdleState();
	auto WalkState() -> void;
	void CageState();


	// 남자, 나 자신
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AJSH_Man* Me;

	// 따라갈 메인 여자 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AJSH_Player* Player;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	float ReachDistance = 150.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=FSM)
	float IdleDistance = 300.0f;

	

	// Cage에 갇히기 전 까지 돌다가, Cage 갇히면 false로 바꿔줘야 함
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Cage = true;

};
