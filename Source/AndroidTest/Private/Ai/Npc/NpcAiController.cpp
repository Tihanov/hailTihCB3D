// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Npc/NpcAiController.h"

#include "Log.h"
#include "Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "HealthPoints/HealthPointsComponent.h"


ANpcAiController::ANpcAiController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANpcAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto NpcAiCharacter = Cast<ANpcAiCharacter>(InPawn);
	check(NpcAiCharacter);

	NpcAiCharacter->OnNpcChangeStateDelegate.AddDynamic(this,
		&ANpcAiController::OnNpcStateChangeCallback);
	
	RerunBehavior();
	if(!NpcAiCharacter->PointsOfInterest.IsEmpty())
		SpawnPoiFromInstance(NpcAiCharacter->PointsOfInterest[0]);

	const auto NpcHpComponent = NpcAiCharacter->GetHpComponent();
	check(NpcHpComponent);
	NpcHpComponent->OnPawnDeathDelegate.AddDynamic(this, &ANpcAiController::OnPawnDeathCallback);
}

void ANpcAiController::OnUnPossess()
{
	const auto NpcAiCharacter = Cast<ANpcAiCharacter>(GetPawn());
	check(NpcAiCharacter);
	Super::OnUnPossess();
	
	NpcAiCharacter->OnNpcChangeStateDelegate.RemoveDynamic(this,
		&ANpcAiController::OnNpcStateChangeCallback);
	CleanupBrainComponent();
}

bool ANpcAiController::SetCurrentPointOfInterest(int32 Index)
{
	const auto NpcAiCharacter = GetControlledAiCharacter();
	if(!NpcAiCharacter->PointsOfInterest.IsValidIndex(Index))
		return false;
	bIsAfterSetOfPoi = true;
	IndexOfCurrentPointOfInterest = Index;
	SpawnPoiFromInstance(NpcAiCharacter->PointsOfInterest[Index]);
	return true;
}

AAiPointOfInterest* ANpcAiController::GetCurrentPointOfInterest() const
{
	return CurrentPoi;
}

void ANpcAiController::SwapToNextInterest()
{
	if(bIsAfterSetOfPoi)
	{
		bIsAfterSetOfPoi = false;
		return;
	}
	if(IsValid(CurrentPoi))
		CurrentPoi->Destroy();
	const auto NpcAiCharacter = GetControlledAiCharacter();
	IndexOfCurrentPointOfInterest += 1;
	if(!NpcAiCharacter->PointsOfInterest.IsValidIndex(IndexOfCurrentPointOfInterest))
		IndexOfCurrentPointOfInterest = 0;
	SpawnPoiFromInstance(NpcAiCharacter->PointsOfInterest[IndexOfCurrentPointOfInterest]);
}

void ANpcAiController::SpawnPoiFromInstance(UAiPointOfInterestInstance* Instance)
{
	const auto Interest = GetWorld()->SpawnActor<AAiPointOfInterest>(Instance->POI_ToSpawnClass);
	Interest->Init(Instance, this);
	CurrentPoi = Interest;
}

void ANpcAiController::RerunBehavior()
{
	const auto NpcAiCharacter = Cast<ANpcAiCharacter>(GetPawn());
	check(NpcAiCharacter);
	if(NpcAiCharacter->GetCurrentBehavior() == nullptr && NpcAiCharacter->GetCurrentState() != ENpcState::Dead)
	{
		ULog::Info(FString::Printf(TEXT("%s state dont have behaviour"),
			*UEnum::GetValueAsString(NpcAiCharacter->GetCurrentState())), LO_Both);
		return;
	}
	
	const auto Behavior = NpcAiCharacter->GetCurrentBehavior();
	if(Behavior)
		RunBehaviorTree(Behavior);
	else
	{
		const auto BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if(BTComp)
			BTComp->StopTree(EBTStopMode::Forced);
	}
}

void ANpcAiController::OnNpcStateChangeCallback(ENpcState NewState, ENpcState OldState)
{
	RerunBehavior();
}

void ANpcAiController::OnPawnDeathCallback(UHealthPointsComponent* HpComponent)
{
	const auto NpcAiCharacter = Cast<ANpcAiCharacter>(GetPawn());
	if(!NpcAiCharacter)
	{
		ULog::Error("!NpcAiCharacter", LO_Both);
		return;
	}

	NpcAiCharacter->SetCurrentState(ENpcState::Dead);
}

ANpcAiCharacter* ANpcAiController::GetControlledAiCharacter() const
{
	const auto ToRet = GetPawn<ANpcAiCharacter>();
	check(ToRet);
	return ToRet;
}

