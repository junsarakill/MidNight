// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "BS_ServerManager.generated.h"

UCLASS()
class MIDNIGHT_API ABS_ServerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABS_ServerManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString defaultUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	TArray<FString> pyPaths;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ReqPostTemp();

	void ResPostTemp(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess);

};
