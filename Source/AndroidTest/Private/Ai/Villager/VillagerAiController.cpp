// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Villager/VillagerAiController.h"

#include "Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "Ai/Villager/VillagerCharacter.h"


AVillagerAiController::AVillagerAiController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVillagerAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto Villager = Cast<AVillagerCharacter>(InPawn);
	check(Villager);
	check(Villager->StartupBehaviorTree);
	check(!Villager->PointsOfInterest.IsEmpty());
	
	RunBehaviorTree(Villager->StartupBehaviorTree);
	SpawnPoiFromInstance(Villager->PointsOfInterest[0]);
}

bool AVillagerAiController::SetCurrentPointOfInterest(int32 Index)
{
	const auto Villager = GetControlledAiCharacter();
	if(!Villager->PointsOfInterest.IsValidIndex(Index))
		return false;
	bIsAfterSetOfPoi = true;
	IndexOfCurrentPointOfInterest = Index;
	SpawnPoiFromInstance(Villager->PointsOfInterest[Index]);
	return true;
}

AAiPointOfInterest* AVillagerAiController::GetCurrentPointOfInterest() const
{
	return CurrentPoi;
}

void AVillagerAiController::SwapToNextInterest()
{
	if(bIsAfterSetOfPoi)
	{
		bIsAfterSetOfPoi = false;
		return;
	}
	if(IsValid(CurrentPoi))
		CurrentPoi->Destroy();
	const auto Villager = GetControlledAiCharacter();
	IndexOfCurrentPointOfInterest += 1;
	if(!Villager->PointsOfInterest.IsValidIndex(IndexOfCurrentPointOfInterest))
		IndexOfCurrentPointOfInterest = 0;
	SpawnPoiFromInstance(Villager->PointsOfInterest[IndexOfCurrentPointOfInterest]);
}

void AVillagerAiController::SpawnPoiFromInstance(UAiPointOfInterestInstance* Instance)
{
	const auto Interest = GetWorld()->SpawnActor<AAiPointOfInterest>(Instance->POI_ToSpawnClass);
	Interest->Init(Instance, this);
	CurrentPoi = Interest;
}


AVillagerCharacter* AVillagerAiController::GetControlledAiCharacter() const
{
	const auto Villager = GetPawn<AVillagerCharacter>();
	check(Villager);
	return Villager;
}

