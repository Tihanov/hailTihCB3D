// Fill out your copyright notice in the Description page of Project Settings.

#include "AndroidTest/Public/Ai/Utils/PointOfInterest/AiPointOfInterest.h"

UAiPointOfInterestInstance::UAiPointOfInterestInstance()
{
	POI_ToSpawnClass = AAiPointOfInterest::StaticClass();
}



void AAiPointOfInterest::Init_Implementation(UAiPointOfInterestInstance* Instance, AVillagerAiController* AiController)
{
	check(Instance);
	InstanceOfSelf = Instance;

	check(AiController);
	VillagerAiController = AiController;
}

bool AAiPointOfInterest::IsComplete_Implementation() const
{
	return true;
}

UAiPointOfInterestInstance* AAiPointOfInterest::GetInstance() const
{
	return InstanceOfSelf;
}

AVillagerAiController* AAiPointOfInterest::GetAiController() const
{
	return VillagerAiController;
}
