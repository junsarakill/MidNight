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
	Owner = GetOwner();
	// ...
}


// Called every frame
void UMH_DropComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsMoving)
	{
		if (Owner)
		{
			FVector CurrentLocation = Owner->GetActorLocation();
			FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
			FVector NewLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;

			// 목표 위치에 거의 도달했는지 확인
			if (FVector::Dist(NewLocation, TargetLocation) < 5.0f)
			{
				NewLocation = TargetLocation;
				bIsMoving = false; // 목표 위치에 도달했으므로 이동 중지
			}

			Owner->SetActorLocation(NewLocation);
		}
	}
}

void UMH_DropComponent::DropBread()
{
	// 소켓으로 변경예정
	// 플레이어의 소켓 이름 
	//FName SocketName = TEXT("YourSocketName");

	// GetOwner()가 캐릭터인지 확인
	//USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();

	//if (MeshComp && MeshComp->DoesSocketExist(SocketName))
	//{
	//FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);


	int32 random = FMath::RandRange(0, 9);

	FTransform posit = GetOwner()->GetActorTransform();
	FVector Scale1(1.f, 1.f, 1.f);
	FVector Scale2(0.7f, 0.7f, 0.7f);
	FVector Location1(3.f, 40.f, 0.f);
	FVector Location2(6.f, 50.f, 0.f);
	FRotator Rotation(0.f, 0.f, 0.f);

	FTransform Transform1(Rotation, posit.GetLocation() + Location1, Scale1);
	FTransform Transform2(Rotation, posit.GetLocation() + Location2, Scale1);
	
	if (random < 3)
	{
		if (Owner)
		{
			AActor* SpawnedActor2 = GetWorld()->SpawnActor<AActor>(Bread, Transform2);
			
			if (SpawnedActor2)
			{
				SpawnedActor2->SetActorRelativeScale3D(Scale2); // 스폰된 액터의 스케일을 설정
			}
			GetWorld()->SpawnActor<AActor>(Bread, Transform1);
		}
	}
	else
	{
		GetWorld()->SpawnActor<AActor>(Bread, Transform1);
	}

	//}
}

void UMH_DropComponent::MovePlayer(float Distance)
{
	if (Owner && !bIsMoving)
	{
		FVector CurrentLocation = Owner->GetActorLocation();
		FVector ForwardVector = Owner->GetActorForwardVector();

		TargetLocation = CurrentLocation + ForwardVector * Distance;
		bIsMoving = true;
	}
}
