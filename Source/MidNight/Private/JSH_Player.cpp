// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Player.h"

// Sets default values
AJSH_Player::AJSH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJSH_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJSH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

