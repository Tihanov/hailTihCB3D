// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/TimeManager.h"

#define RESULT_NORMALIZE(RESULT) ((RESULT) > 0) ? 1 : -1;

int32 ClockCompare(const FClock& A, const FClock& B)
{
	int32 Result = A.Hour - B.Hour;
	if(Result != 0)
		return RESULT_NORMALIZE(Result);
	
	Result = A.Minute - B.Minute;
	if(Result != 0)
		return RESULT_NORMALIZE(Result);
	
	Result = A.Second - B.Second;
	if(Result != 0)
		return RESULT_NORMALIZE(Result);
	return 0;
}

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

void ATimeManager::SetCurrentTimeInSeconds(float Time)
{
	CurrentTime = Time;
	if(CurrentTime >= MAX_TIME)
		CurrentTime -= MAX_TIME;
}

float ATimeManager::GetCurrentTimeInSeconds() const
{
	return CurrentTime;
}

void ATimeManager::SetCurrentTime(FClock Clock)
{
	CurrentTime
		= static_cast<float>(Clock.Hour) * 3600.f
		+ static_cast<float>(Clock.Minute) * 60.f
		+ static_cast<float>(Clock.Second);
}

FClock ATimeManager::GetCurrentTime() const
{
	const int32 Time = CurrentTime;
	FClock ToRet;
	ToRet.Hour = Time / 3600;
	ToRet.Minute = Time % 3600 / 60;
	ToRet.Second = Time % 3600 % 60;
	return ToRet;
}
