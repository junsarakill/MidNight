// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_ServerManager.h"
#include "BS_Utility.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

// Sets default values
ABS_ServerManager::ABS_ServerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 기본 경로 가져오기
	pyDefaultPath = FPaths::ProjectContentDir() + TEXT("Scripts/");
	pyNames.SetNum(4);
}

// Called when the game starts or when spawned
void ABS_ServerManager::BeginPlay()
{
	Super::BeginPlay();
	
	// ReqPostTemp();
}

// Called every frame
void ABS_ServerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABS_ServerManager::ReqPostTemp()
{
	// json 만들기
	TMap<FString, FString> data;
	data.Add("name", "doremi");
	data.Add("age","99");
	data.Add("height","675");

	FString json = UBS_Utility::MakeJson(data);
	// 
	FString resultUrl = defaultUrl;
	// 1. http 모듈 생성
	FHttpModule* http = &FHttpModule::Get();

	// 2. 요청 정보 설정
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpReq = http->CreateRequest();

	// 3. 응답 함수 연결
	httpReq->OnProcessRequestComplete().BindUObject(this, &ABS_ServerManager::ResPostTemp);
	httpReq->SetContentAsString(json);
	httpReq->SetURL(resultUrl);
	httpReq->SetVerb(TEXT("POST")); // == METHOD
	httpReq->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // 헤더 설정 json

	// 서버 요청
	httpReq->ProcessRequest();
}

void ABS_ServerManager::RunQTEScript(EQTEType type)
{
	// 해당 스크립트 이름 찾아서 실행
	for(auto pyName : pyNames)
	{
		if(pyName.type == type)
		{
			FString pyPath = pyDefaultPath + pyName.pyName;
			RunPythonScript(pyPath);
			break;
		}
	}
}

void ABS_ServerManager::ReqGetValue(EQTEType type)
{
	FString resultUrl = defaultUrl;
	// 해당 타입 찾아서 url 완성
	for(auto urlName : urlNames)
	{
		if(urlName.type == type)
		{
			resultUrl += urlName.urlName;
			break;
		}
	}

	// 1. http 모듈 생성
	FHttpModule* http = &FHttpModule::Get();

	// 2. 요청 정보 설정
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpReq = http->CreateRequest();

	// 3. 응답 함수 연결
	httpReq->OnProcessRequestComplete().BindUObject(this, &ABS_ServerManager::ResGetValue);
	httpReq->SetURL(resultUrl);
	httpReq->SetVerb(TEXT("GET")); // == METHOD
	httpReq->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // 헤더 설정 json

	// 서버 요청
	httpReq->ProcessRequest();


}

void ABS_ServerManager::ResGetValue(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess)
{
	// 응답성공
	if(isSuccess && res.IsValid())
	{
		FString resStr = res->GetContentAsString();
		// @@ 해당 값 가져오기
		// @@ 값으로 뭔가 하기

		// FString result = UJK_JsonLib::JsonParse(resStr);
		// FString result = resStr;

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Response : %s"), *resStr));

		// mainUI->SetLogText(result);
	}
	// 응답실패
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("요청 실패"));
	}
}

void ABS_ServerManager::ResPostTemp(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess)
{
	// 응답성공
	if(isSuccess && res.IsValid())
	{
		FString resStr = res->GetContentAsString();

		// FString result = UJK_JsonLib::JsonParse(resStr);
		// FString result = resStr;

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Response : %s"), *resStr));

		// mainUI->SetLogText(result);
	}
	// 응답실패
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("요청 실패"));
	}
}
