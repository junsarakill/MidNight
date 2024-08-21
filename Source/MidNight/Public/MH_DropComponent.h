// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MH_DropComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIDNIGHT_API UMH_DropComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMH_DropComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "MySettings")
	TSubclassOf<AActor> Bread; 

	UFUNCTION(BlueprintCallable)
	void DropBread();

	// 이동 함수
	UFUNCTION(BlueprintCallable, Category="Movement")
	void MovePlayer();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Distance=300.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsMoving;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MoveSpeed = 100.f;

	UPROPERTY()
	class AMH_Scene01GameMode* GM01;
	
	UPROPERTY()
	class AMH_Player* OwningPawn;
		
};
