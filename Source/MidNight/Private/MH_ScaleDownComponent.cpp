// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_ScaleDownComponent.h"

#include "MH_Scene02GameMode.h"

// Sets default values for this component's properties
UMH_ScaleDownComponent::UMH_ScaleDownComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMH_ScaleDownComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	GM02 = Cast<AMH_Scene02GameMode>(GetWorld()->GetAuthGameMode());
	
}


// Called every frame
void UMH_ScaleDownComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMH_ScaleDownComponent::ScaleDownSnack()
{
	
	 AActor* Owner = GetOwner();
    if (Owner)
    {
        // 현재 스케일을 가져옵니다.
        FVector CurrentScale = Owner->GetActorScale3D();
        
        // 스케일을 줄입니다.
        FVector NewScale = CurrentScale * 0.5f; // 90%로 줄이기
        
        // 스케일이 너무 작아지면 삭제
        if (NewScale.SizeSquared() <= 0.3f)
        {
            GM02->Scene02+=1;
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Scene02+!"));
            GM02->CheckLevelTransition();
            Owner->Destroy(); // 사라짐
        }
        else
        {
            // 새로운 스케일을 설정
            Owner->SetActorScale3D(NewScale);
        }
    	
    }
}
