// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_DropComponent.h"

// Sets default values for this component's properties
UMH_DropComponent::UMH_DropComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMH_DropComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMH_DropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMH_DropComponent::DropBread()
{
	// 소켓으로 변경예정
	// 플레이어의 소켓 이름 
	//FName SocketName = TEXT("YourSocketName");

	// GetOwner()가 캐릭터인지 확인
	AActor* Owner = GetOwner();
	//USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();

	//if (MeshComp && MeshComp->DoesSocketExist(SocketName)) // 소켓 존재 여부 확인
	//{
		// 소켓의 위치
		//FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);

	
		FTransform posit = GetOwner()->GetActorTransform();
		GetWorld()->SpawnActor<AActor>(Bread, posit);
	//}
	
}

