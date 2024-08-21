// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_ServerManager.h"
#include "BS_Utility.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h" 
#include "MH_Player.h"
// #include "HAL/RunnableThread.h" 
// #include "PythonScriptPlugin/PythonScriptPlugin.h"
// #include "PythonScriptPlugin.h" // PythonScriptPlugin 관련 기본 헤더
// #include "PythonScriptPlugin/Public/IPythonScriptPlugin.h"


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

	// tcp 통신 결과 받기
	ReceiveData();

	// TestRecieveData();
}



#pragma region 미사용

// void ABS_ServerManager::ReqGetValue(EQTEType type)
// {
// 	FString resultUrl = defaultUrl;
// 	// 해당 타입 찾아서 url 완성
// 	for(auto urlName : urlNames)
// 	{
// 		if(urlName.type == type)
// 		{
// 			resultUrl += urlName.urlName;
// 			break;
// 		}
// 	}

// 	// 1. http 모듈 생성
// 	FHttpModule* http = &FHttpModule::Get();

// 	// 2. 요청 정보 설정
// 	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpReq = http->CreateRequest();

// 	// 3. 응답 함수 연결
// 	httpReq->OnProcessRequestComplete().BindUObject(this, &ABS_ServerManager::ResGetValue);
// 	httpReq->SetURL(resultUrl);
// 	httpReq->SetVerb(TEXT("GET")); // == METHOD
// 	httpReq->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // 헤더 설정 json

// 	// 서버 요청
// 	httpReq->ProcessRequest();


// }

// void ABS_ServerManager::ResGetValue(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess)
// {
// 	// 응답성공
// 	if(isSuccess && res.IsValid())
// 	{
// 		FString resStr = res->GetContentAsString();
// 		// @@ 해당 값 가져오기
// 		// @@ 값으로 뭔가 하기

// 		// FString result = UJK_JsonLib::JsonParse(resStr);
// 		// FString result = resStr;

// 		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Response : %s"), *resStr));

// 		// mainUI->SetLogText(result);
// 	}
// 	// 응답실패
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("요청 실패"));
// 	}
// }

// void ABS_ServerManager::ResPostTemp(FHttpRequestPtr req, FHttpResponsePtr res, bool isSuccess)
// {
// 	// 응답성공
// 	if(isSuccess && res.IsValid())
// 	{
// 		FString resStr = res->GetContentAsString();

// 		// FString result = UJK_JsonLib::JsonParse(resStr);
// 		// FString result = resStr;

// 		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Response : %s"), *resStr));

// 		// mainUI->SetLogText(result);
// 	}
// 	// 응답실패
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("요청 실패"));
// 	}
// }

#pragma endregion

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

void ABS_ServerManager::RunAsyncScript(EQTEType type)
{
	// 해당 스크립트 이름 찾아서 실행
	for(auto pyName : pyNames)
	{
		if(pyName.type == type)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%s 타입 실행"), *UEnum::GetValueAsString(type)));
			FString pyPath = pyDefaultPath + pyName.pyName;
			RunAsyncPythonScript(pyPath);
			break;
		}
	}
}

void ABS_ServerManager::RunAsyncPythonScript(const FString &path)
{
	FString PythonExePath = TEXT("C:/Program Files/python/python.exe");
    if (FPaths::FileExists(PythonExePath) && FPaths::FileExists(path))
    {
        // Python 프로세스를 실행
        FPlatformProcess::CreateProc(*PythonExePath, *path, true, false, false, nullptr, 0, nullptr, nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("자체 python에서 실행"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Python executable or script not found."));
    }
}

void ABS_ServerManager::CreateClient(FString ip, int32 port)
{
	// 기본 값 안되니깐 설정
	if(ip == TEXT(""))
	{
		ip = serverIP;
	}
	if(port == 0)
	{
		port = serverPort;
	}

	getData = -1;



    // 클라이언트 소켓 생성
    ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP Client"), false);
    ServerAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    // 서버 IP와 포트 설정
    FIPv4Address Ip;
    FIPv4Address::Parse(ip, Ip); // 서버의 IP 주소 (예: localhost)
    ServerAddr->SetIp(Ip.Value); // IP 주소 설정
    ServerAddr->SetPort(port); // 서버 포트:

    // 서버에 연결
	FString connStr = ClientSocket->Connect(*ServerAddr) ? TEXT("서버에 연결되었습니다.") : TEXT("서버 연결 실패.");

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *connStr));
}

void ABS_ServerManager::ReceiveData()
{
    if (ClientSocket && ClientSocket->GetConnectionState() == SCS_Connected)
    {
        uint32 Size;
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("야호 연결됬다\n데이터 확인 %d"), ClientSocket->HasPendingData(Size)));
        while (ClientSocket->HasPendingData(Size))
        {
            TArray<uint8> ReceivedData;
            ReceivedData.SetNumUninitialized(FMath::Min(Size, 1024u));

            // 데이터 수신
            int32 BytesRead = 0;
            if (ClientSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead))
            {
                if (BytesRead > 0)
                {
                    FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
                    GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("Received: %s"), *ReceivedString));
					// @@ 데이터 처리
					if(ReceivedString.Contains(TEXT("1")))
					{
						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("모션 캡쳐 성공")));
						getData = 1;
						// 데이터를 플레이어에게 전달
						SendDateToPlayer(getData);
						// // 통신 종료
						// Disconnect();
					}
                }
                else
				{
                    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("No data received."));
				}
            }
            else
			{
                GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Failed to receive data."));
			}
        }
    }
    else
	{
        GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Red, TEXT("Socket is not connected or does not exist."));
	}
}

void ABS_ServerManager::Disconnect()
{
	if(ClientSocket)
	{
		ClientSocket->Close();
		ClientSocket = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("서버와 연결 종료"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("소켓 없음"));
	}
	
}

void ABS_ServerManager::SendDateToPlayer(int32 data)
{
	if(!player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("플레이어 없음"));
		return;
	}
	
	player->ReceiveDataFromSM(data);
}

void ABS_ServerManager::TestRecieveData()
{
	if(!ClientSocket)
		return;

	// uint32 pendingSize;
	// if (!ClientSocket->HasPendingData(pendingSize) )
	// {
	// 	return;
	// }

	TArray<uint8> headerInfo;
	int32 HeaderSize = 1;
	headerInfo.AddZeroed(HeaderSize);

	int readNum = 0;

	bool bSuccess = ClientSocket->Recv(headerInfo.GetData(), HeaderSize, readNum, ESocketReceiveFlags::Type::WaitAll);

	if (readNum > 0)
	{
		TArray<uint8> BodyInfo;
		int32 BodySize =headerInfo[0];
		BodyInfo.AddZeroed(HeaderSize);

		int32 Offset = 0;
		uint8* results = BodyInfo.GetData();
		while (BodySize > 0)
		{
			int32 NumRead = 0;
			ClientSocket->Recv(results + Offset, BodySize, NumRead);
			
			if (NumRead <= 0)
			{
				break;
			}

			Offset += NumRead;
			BodySize -= NumRead;
		}

		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Green, FString::Printf(TEXT("데이터 확인 %d"), BodyInfo[0]));
	}

}

