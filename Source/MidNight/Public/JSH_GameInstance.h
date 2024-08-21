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

	// Player 여자
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GameProgress = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndVector = FVector(610.0f	, -1260.0f, 96.0f); 
	
	// Player 남자
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ManEndVector = FVector(610.0f	, -1260.0f, 96.0f);


	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BookPoint = 0;
	
};
