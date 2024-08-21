// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MH_Scene04GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHT_API AMH_Scene04GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	bool Scene04 = false;
	
	UFUNCTION(BlueprintCallable ,BlueprintImplementableEvent)
	void TriggerLevelTransition();

	UFUNCTION(BlueprintCallable)
	void CheckLevelTransition();
};
