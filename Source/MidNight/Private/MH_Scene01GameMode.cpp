// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_Scene01GameMode.h"

void AMH_Scene01GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMH_Scene01GameMode::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
}

void AMH_Scene01GameMode::CheckLevelTransition()
{
	if (Scene01 == 4)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMH_Scene01GameMode::TriggerLevelTransition, 3.0f, false);
	}
}
