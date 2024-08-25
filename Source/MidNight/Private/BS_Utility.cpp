// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_Utility.h"
#include "Json.h"
#include "JsonUtilities.h"
#include <Kismet/GameplayStatics.h>
#include <BS_ServerManager.h>
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"

FString UBS_Utility::ParseJson(const FString& jsonData)
{
    // 리더기 만들기
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonData);
    // 담을 변수 선언
    TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
    // 해석
    FString returnStr = TEXT("");
    if(FJsonSerializer::Deserialize(reader, result))
    {
        // 모든 필드 반복
        for(const auto& field : result->Values)
        {
            if(field.Value->Type == EJson::String)
                returnStr += field.Value->AsString() + TEXT(", ");
        }
        // 마지막에 추가된 쉼표와 공백 제거
        if (!returnStr.IsEmpty())
        {
            returnStr.RemoveAt(returnStr.Len() - 2, 2); // 쉼표와 공백 제거
        }

        /*
        // XXX 만약 json 배열이 들어온다면 내부를 배열로 가져옴
        TArray<TSharedPtr<FJsonValue>> parseDataList = result->GetArrayField(TEXT("items"));
        for(auto data : parseDataList)
        {
            FString bookName = data->AsObject()->GetStringField(TEXT("bk_nm"));
            FString authName = data->AsObject()->GetStringField(TEXT("aut_nm"));
            returnStr.Append(FString::Printf(TEXT("%s %s"), *bookName, *authName));
        }
        */
    }
    // 반환
    return returnStr;
}

FString UBS_Utility::MakeJson(const TMap<FString, FString> source)
{
    TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
    for(TPair<FString, FString> pair : source)
    {
        jsonObj->SetStringField(pair.Key, pair.Value);
    }
    FString jsonData;
    // 읽개 만들기
    TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&jsonData);
    if(FJsonSerializer::Serialize(jsonObj.ToSharedRef(), writer))
    {

    }

    return jsonData;
}

void UBS_Utility::ReqTemp()
{
    // 임시로 데이터 받아보기
    // http 모듈 생성
	FHttpModule* http = &FHttpModule::Get();

	// 요청 정보 설정
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpReq = http->CreateRequest();

	// 응답 함수 연결
	// httpReq->OnProcessRequestComplete().BindUObject(this, &UBS_Utility::ResTemp);
	// httpReq->SetURL(url);
	httpReq->SetVerb(TEXT("GET")); // == METHOD
	httpReq->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // 헤더 설정 json

	// 서버 요청
	httpReq->ProcessRequest();
}

void UBS_Utility::ResTemp(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess)
{
    // 응답성공
	if(isSuccess && res.IsValid())
	{
		FString resStr = res->GetContentAsString();
        // 데이터 받기
		FString result = UBS_Utility::ParseJson(resStr);
        
		// UE_LOG(LogTemp, Log, TEXT("Response: %s"), *result);
        // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *result));

		// mainUI->SetLogText(result);
	}
	// 응답실패
	else
        // 
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("요청 실패"));
	}
}

// ABS_ServerManager* UBS_Utility::GetServerManager(UWorld* world)
// {
//     auto* sm = Cast<ABS_ServerManager>(
//         UGameplayStatics::GetActorOfClass(world, ABS_ServerManager::StaticClass()));
//     if(sm)
//     {
//         //해당 액터로 뭔가 하기
        
//     }

//     return sm;
// }
















void UBS_Utility::ConnectAndReceiveData()
{
    // 소켓 생성
    // FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

    // // IP 주소와 포트 설정
    // TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    // bool bIsValid;
    // RemoteAddress->SetIp(TEXT("192.168.1.59"), bIsValid);
    // RemoteAddress->SetPort(65432);

    // // 연결 시도
    // bool bConnected = Socket->Connect(*RemoteAddress);
    // if (bConnected)
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Connected to 192.168.1.59:65432")));

    //     uint32 BufferSize = 1024;
    //     TArray<uint8> ReceiveBuffer;
    //     ReceiveBuffer.SetNumUninitialized(BufferSize);

    //     int32 BytesRead = 0;

    //     // 데이터 수신
    //     while (Socket->Recv(ReceiveBuffer.GetData(), ReceiveBuffer.Num(), BytesRead))
    //     {
    //         // 받은 데이터 처리
    //         FString ReceivedData = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceiveBuffer.GetData())));
    //         GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Received Data: %s"), *ReceivedData));

    //         // 수신된 바이트 수가 0이면 연결이 종료된 것
    //         if (BytesRead == 0)
    //         {
    //             break;
    //         }
    //     }
    // }
    // else
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Failed to connect to 192.168.1.59:65432")));
    // }

    // // 소켓 닫기
    // Socket->Close();
    
}
