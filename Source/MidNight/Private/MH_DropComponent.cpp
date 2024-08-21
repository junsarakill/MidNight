// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_DropComponent.h"

#include "MH_Scene01GameMode.h"
#include "TimerManager.h"
#include "MH_Player.h"

// Sets default values for this component's properties
UMH_DropComponent::UMH_DropComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 기본값 설정
	bIsMoving = false;
	MoveSpeed = .7f;
}


// Called when the game starts
void UMH_DropComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPawn = Cast<AMH_Player>(GetOwner());
	
	GM01 = Cast<AMH_Scene01GameMode>(GetWorld()->GetAuthGameMode());
}


// Called every frame
void UMH_DropComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsMoving && OwningPawn)
	{
		// 목표 위치까지의 방향 벡터 계산
		FVector CurrentLocation = OwningPawn->GetActorLocation();
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

		// 목표 방향으로 이동
		OwningPawn->AddMovementInput(Direction, MoveSpeed);

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("ddd"));

		// 목표 위치에 도달했는지 확인
		if (FVector::Dist(CurrentLocation, TargetLocation) < 10.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("asd"));
			bIsMoving = false; // 목표 위치에 도달하면 이동 중지
			if(OwningPawn)
				OwningPawn->StartQTE();
		}
	}
}

void UMH_DropComponent::DropBread()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("빵 던지기"));
	// 소켓으로 변경예정
	// 플레이어의 소켓 이름 
	//FName SocketName = TEXT("YourSocketName");

	// GetOwner()가 캐릭터인지 확인
	//USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();

	//if (MeshComp && MeshComp->DoesSocketExist(SocketName))
	//{
	//FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	AActor* Owner = GetOwner();

	int32 random = FMath::RandRange(0, 9);

	FTransform posit = GetOwner()->GetActorTransform();
	FVector Scale1(1.f, 1.f, 1.f);
	FVector Scale2(0.8f, 0.8f, 0.8f);
	FVector Location1(3.f, 40.f, 0.f);
	FVector Location2(6.f, 50.f, 0.f);
	FRotator Rotation(0.f, 0.f, 0.f);

	FTransform Transform1(Rotation, posit.GetLocation() + Location1, Scale1);
	FTransform Transform2(Rotation, posit.GetLocation() + Location2, Scale1);

	if (DropAnimPlay)
	{
		/*
		UAnimInstance* AnimInstance = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(MontageToPlay))
		{
			AnimInstance->Montage_Play(MontageToPlay);
		}*/
	}
	
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

	if(GM01)
	{
		GM01->Scene01 +=1;
		GM01->CheckLevelTransition();
	}
}

void UMH_DropComponent::MovePlayer()
{
	if (OwningPawn && !bIsMoving)
	{
		FVector CurrentLocation = OwningPawn->GetActorLocation();
		FVector ForwardVector = OwningPawn->GetActorForwardVector();

		// 목표 위치 설정
		TargetLocation = CurrentLocation + ForwardVector * Distance;
		bIsMoving = true;
		
	}
}
