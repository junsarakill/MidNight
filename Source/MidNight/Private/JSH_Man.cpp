// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Man.h"

// Sets default values
AJSH_Man::AJSH_Man()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJSH_Man::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_Man::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJSH_Man::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

