// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/TimeManager.h"


ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime * CountOfGameSecondsInOneRealSecond;
	if(CurrentTime >= MAX_TIME)
		CurrentTime -= MAX_TIME;
}

void ATimeManager::SetCurrentTime(float Time)
{
	CurrentTime = Time;
	if(CurrentTime >= MAX_TIME)
		CurrentTime -= MAX_TIME;
}

float ATimeManager::GetCurrentTime() const
{
	return CurrentTime;
}

