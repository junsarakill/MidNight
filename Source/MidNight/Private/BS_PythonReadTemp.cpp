#include "BS_PythonReadTemp.h" 
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
// #include "PythonScriptPlugin.h"

// Sets default values
ABS_PythonReadTemp::ABS_PythonReadTemp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABS_PythonReadTemp::BeginPlay()
{
	Super::BeginPlay();
	
	RunPython();
}

// Called every frame
void ABS_PythonReadTemp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABS_PythonReadTemp::RunPython()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("함수 실행"));

    // Python 스크립트 경로 설정
    FString PythonScriptPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Scripts/asd.py"));

    // Python 스크립트 실행
    // if (FPythonScriptPlugin::Get()->ExecPythonCommand(*FString::Printf(TEXT("exec(open('%s').read())"), *PythonScriptPath)))
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Python script executed successfully."));
    // }
    // else
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Failed to execute Python script."));
    // }
}
