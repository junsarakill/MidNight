// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_ConnTemp.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

// Sets default values
ABS_ConnTemp::ABS_ConnTemp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABS_ConnTemp::BeginPlay()
{
	Super::BeginPlay();

	// RunPython();
	
}

// Called every frame
void ABS_ConnTemp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FPlatformProcess::IsProcRunning(processHandle))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Python script is still running."));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Python script has finished."));
    }
}

void ABS_ConnTemp::RunPython()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("함수 실행"));
	// 파이썬 실행 파일 경로
    FString PythonExecutable = TEXT("C:/Program Files/Epic Games/UE_5.4/Engine/Binaries/ThirdParty/Python3/Win64/python.exe"); // 파이썬 경로를 수정하세요.
	// FString PythonExecutable = TEXT("C:/Program Files/python/python.exe"); // 파이썬 경로를 수정하세요.
    // 실행할 스크립트 경로
    FString ScriptPath = FPaths::ProjectContentDir() + TEXT("Scripts/give.py"); // 스크립트 경로를 수정하세요.

    // 실행할 명령어 (스크립트 경로 포함)
    FString Command = FString::Printf(TEXT("\"%s\" \"%s\""), *PythonExecutable, *ScriptPath);

    // 프로세스 실행
	processHandle = FPlatformProcess::CreateProc(*PythonExecutable, *ScriptPath, true, false, false, nullptr, 0, nullptr, nullptr);

    if (processHandle.IsValid())
    {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Python script is running successfully."));
    }
    else
    {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Failed to run Python script."));
    }
}
