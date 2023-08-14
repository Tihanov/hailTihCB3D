// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/TimeManager.h"

#include "Log.h"

#define RESULT_NORMALIZE(RESULT) ((RESULT) > 0) ? 1 : -1;

const FClock FClock::MinClock = {0, 0, 0}; 
const FClock FClock::MaxClock = {24, 0, 0};

int32 FClock::Compare(const FClock& A, const FClock& B)
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

bool FClock::IsBetween(const FClock& X, const FClock& A, const FClock& B)
{
	FClock A_ = A;
	FClock B_ = B;
	FClock X_ = X;
	if(FClock::Compare(A_, B_) == 0)
		return FClock::Compare(A_, B_) == 0;
	if(FClock::Compare(A_, B_) == 1)
		B_ = FClock::PureAdd(FClock::MaxClock, B_);
	if(FClock::Compare(A_, X_) == 1)
		X_ = FClock::PureAdd(FClock::MaxClock, X_);	
	return FClock::Compare(X_, A_) != -1 && FClock::Compare(X_, B_) != 1;
}

FClock FClock::PureAdd(const FClock& A, const FClock& B)
{
	FClock ToRet;
	ToRet.Hour = A.Hour + B.Hour;
	ToRet.Minute= A.Minute + B.Minute;
	ToRet.Second = A.Second+ B.Second;
	return ToRet;
}

float FClock::ToSeconds(const FClock& A)
{
	return static_cast<float>(A.Hour) * 3600.f
	+ static_cast<float>(A.Minute) * 60.f
	+ static_cast<float>(A.Second);
}

FClock FClock::FromSeconds(float A)
{
	const int32 A_ = A;
	FClock ToRet;
	ToRet.Hour = A_ / 3600;
	ToRet.Minute = A_ % 3600 / 60;
	ToRet.Second = A_ % 3600 % 60;
	return ToRet;
}


ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeManager::BeginPlay()
{
	Super::BeginPlay();
	if(bStartFromDebugTime)
		SetCurrentTime(DebugStartTime);
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
	CurrentTime = FClock::ToSeconds(Clock);
}

FClock ATimeManager::GetCurrentTime() const
{
	return FClock::FromSeconds(CurrentTime);
}

