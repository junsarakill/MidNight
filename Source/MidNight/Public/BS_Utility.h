// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Http.h"
#include "BS_Utility.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHT_API UBS_Utility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// json을 사용가능하게 받기
	static FString ParseJson(const FString& jsonData);
	// 데이터 json 화
	static FString MakeJson(const TMap<FString, FString> source);

	// XXX 테스트 용 http 요청
	static void ReqTemp();
	static void ResTemp(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess);
	
};
