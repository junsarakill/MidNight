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

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	// FString defaultUrl;
	//tcp ip, port
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString serverIP = TEXT("127.0.0.1");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	int32 serverPort = 65432;

	// 파이썬 스크립트 기본 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	FString pyDefaultPath;
	// 스크립트 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	TArray<FPyName> pyNames;
	// url 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Values")
	TArray<FUrlName> urlNames;

	// tcp 통신 소켓
	// 클라이언트 소켓
	FSocket* ClientSocket;
	// 서버 주소
    TSharedPtr<FInternetAddr> ServerAddr;
	
	

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

	UFUNCTION(BlueprintCallable)
	void RunAsyncScript(EQTEType type);
	UFUNCTION(BlueprintCallable)
	void RunAsyncPythonScript(const FString& path);


	// 해당 QTE 값 요청
	// void ReqGetValue(EQTEType type);

	// // QTE 값 받기
	// void ResGetValue(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess);



	// void ReqPostTemp();

	// void ResPostTemp(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess);

	UFUNCTION(BlueprintCallable)
	// 서버와 연결하기
    void CreateClient();
	// 데이터 받기
	UFUNCTION(BlueprintCallable)
    void ReceiveData();
	// 서버와 연결끊기
	UFUNCTION(BlueprintCallable)
	void Disconnect();
	

	
};
