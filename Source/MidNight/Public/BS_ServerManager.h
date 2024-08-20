// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "BS_ServerManager.generated.h"

UENUM(BlueprintType)
enum class EQTEType : uint8
{
	THROW
	,EATING
	,CLEAN
	,GIVE
};
// 스크립트 이름 구조체
USTRUCT(BlueprintType)
struct FPyName
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	EQTEType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString pyName;

	FPyName() : type(EQTEType::THROW), pyName(TEXT("")) {}
};
// 엔드포인트 이름 구조체
USTRUCT(BlueprintType)
struct FUrlName
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	EQTEType type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString urlName;

	FUrlName() : type(EQTEType::THROW), urlName(TEXT("")) {}
};

UCLASS()
class MIDNIGHT_API ABS_ServerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABS_ServerManager();

	// fast api url 기본
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString defaultUrl;

	// 파이썬 스크립트 기본 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString pyDefaultPath;
	// 스크립트 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	TArray<FPyName> pyNames;
	// url 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	TArray<FUrlName> urlNames;
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 해당 py 스크립트 실행
	UFUNCTION(BlueprintCallable)
	void RunQTEScript(EQTEType type);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RunPythonScript(const FString& path);


	// 해당 QTE 값 요청
	void ReqGetValue(EQTEType type);

	// QTE 값 받기
	void ResGetValue(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess);



	void ReqPostTemp();

	void ResPostTemp(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess);

};
