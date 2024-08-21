// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MH_Player.generated.h"

UCLASS()
class MIDNIGHT_API AMH_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMH_Player();

	// 게임 진행도 int : 게임 인스턴스에서 받을 것
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	int32 gameIdx = 0;
	// qte 중인지 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	bool onQTE = false;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 해당 함수로 행동 시작
	UFUNCTION(BlueprintCallable)
	virtual void StartQTE();
	// 모션 캡처 시작
	UFUNCTION(BlueprintCallable)
	virtual void StartMotionCapture(EQTEType type);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CaptureMotion(ABS_ServerManager* sm);

	// 데이터 받기
	UFUNCTION(BlueprintCallable)
	void ReceiveDataFromSM(int32 data);

	// 행동 이벤트
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayerAct(int32 value);
	

};
