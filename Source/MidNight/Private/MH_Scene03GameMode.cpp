// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_Scene03GameMode.h"


void AMH_Scene03GameMode::CheckLevelTransition()
{
	if(Scene03)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMH_Scene03GameMode::TriggerLevelTransition, 2.0f, false);
	}
}
