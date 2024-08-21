// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MH_Scene02GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHT_API AMH_Scene02GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	int32 Scene02 = 0;
	
	UFUNCTION(BlueprintCallable ,BlueprintImplementableEvent)
	void TriggerLevelTransition();

	UFUNCTION(BlueprintCallable)
	void CheckLevelTransition();
	
};
