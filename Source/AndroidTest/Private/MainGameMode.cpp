// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

#include "AndroidTest/Public/Ai/Utils/TimeManager.h"

AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	TimeManagerClass = ATimeManager::StaticClass();
}

void AMainGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	TimeManager = GetWorld()->SpawnActor<ATimeManager>(TimeManagerClass);
	check(TimeManager);
}

ATimeManager* AMainGameMode::GetTimeManager() const
{
	return TimeManager;
}
