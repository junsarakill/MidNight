// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JSH_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHT_API UJSH_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GameProgress = 0;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndVector = FVector(610.0f	, -1260.0f, 96.0f); 
};
