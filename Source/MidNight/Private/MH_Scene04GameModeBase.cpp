// Fill out your copyright notice in the Description page of Project Settings.


#include "MH_Scene04GameModeBase.h"

void AMH_Scene04GameModeBase::CheckLevelTransition()
{
	if(Scene04)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("ddd"));
		GetWorldTimerManager().SetTimer(TimerHandle, this, & AMH_Scene04GameModeBase::TriggerLevelTransition, 2.0f, false);
	}
}
