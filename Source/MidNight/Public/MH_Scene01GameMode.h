// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MH_Scene01GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHT_API AMH_Scene01GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

public:
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	int32 Scene01 = 0;
	
	UFUNCTION(BlueprintCallable ,BlueprintImplementableEvent)
	void TriggerLevelTransition();

	UFUNCTION(BlueprintCallable)
	void CheckLevelTransition();
	
};
