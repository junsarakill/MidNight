// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MH_Scene03GameMode.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHT_API AMH_Scene03GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	bool Scene03 = false;
	
	UFUNCTION(BlueprintCallable ,BlueprintImplementableEvent)
	void TriggerLevelTransition();
	
	UFUNCTION(BlueprintCallable)
	void CheckLevelTransition();
	
	
};
