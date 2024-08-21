// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_Scene02GameMode.h"


void AMH_Scene02GameMode::CheckLevelTransition()
{
	if (Scene02 == 2)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMH_Scene02GameMode::TriggerLevelTransition, 3.0f, false);
	}
}

