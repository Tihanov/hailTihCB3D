// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/PointOfInterest/POI_ClockAndPositionInstance.h"

#include "Log.h"
#include "MainGameMode.h"

UPOI_ClockAndPositionInstance::UPOI_ClockAndPositionInstance()
{
	POI_ToSpawnClass = APOI_ClockAndPosition::StaticClass();
}



APOI_ClockAndPosition::APOI_ClockAndPosition()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APOI_ClockAndPosition::Init_Implementation(UAiPointOfInterestInstance* Instance,
                                                AVillagerAiController* AiController)
{
	Super::Init_Implementation(Instance, AiController);

	TimeManagerSoft = GetTimeManagerFromGameMode();
	
	const auto TimeManager = TimeManagerSoft.Get();
	check(TimeManager);
	const auto Inst = GetInstance<UPOI_ClockAndPositionInstance>();
	check(Inst);
	
	if(ClockCompare(TimeManager->GetCurrentTime(), Inst->Begin) == -1 ||
		ClockCompare(TimeManager->GetCurrentTime(), Inst->End) == 1)
	{
		bIsCompleteCausedByClock = true;
		OnComplete();
	}
}

void APOI_ClockAndPosition::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto TimeManager = TimeManagerSoft.Get();
	check(TimeManager);
	const auto Inst = GetInstance<UPOI_ClockAndPositionInstance>();
	check(Inst);

	if(ClockCompare(TimeManager->GetCurrentTime(), Inst->End) == 1)
	{
		bIsCompleteCausedByClock = true;
		OnComplete();
	}
}

void APOI_ClockAndPosition::OnComplete()
{
	if(IsComplete_Implementation())
		return;
	const auto TimeManager = TimeManagerSoft.Get();
	check(TimeManager);
	const auto Inst = GetInstance<UPOI_ClockAndPositionInstance>();
	check(Inst);
	
	if(ClockCompare(TimeManager->GetCurrentTime(), Inst->End) != 1 && !Inst->bOnArrivedUseTimeout)
		return;
	
	Super::OnComplete();
}

ATimeManager* APOI_ClockAndPosition::GetTimeManagerFromGameMode() const
{
	const auto GameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();
	check(GameMode);
	const auto TimeManager = GameMode->GetTimeManager();
	check(TimeManager);
	return TimeManager;
}
