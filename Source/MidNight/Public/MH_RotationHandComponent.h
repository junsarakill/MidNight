// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MH_RotationHandComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIDNIGHT_API UMH_RotationHandComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMH_RotationHandComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// 입력값에 따라 손을 회전시키는 함수
	UFUNCTION(BlueprintCallable)
	void RotateHand(int32 InputValue);
	
	UFUNCTION(BlueprintCallable)
	void ResetRotation();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator InitialRotation; // 초기 회전 값
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator TargetRotation; // 목표 회전 값
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* CurrentHandActor; // 현재 회전 중인 손 액터
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsRotating; // 회전 중인지 여부

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTimerHandle TimerHandle; // 타이머 핸들러
};
