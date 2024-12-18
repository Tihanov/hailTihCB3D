// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_GetCurrentPointOfInterest.h"

#include "Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "Ai/Npc/NpcAiController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetCurrentPointOfInterest::UBTTask_GetCurrentPointOfInterest()
{
	NodeName = TEXT("Get Current Point Of Interest");
}

EBTNodeResult::Type UBTTask_GetCurrentPointOfInterest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto AiController = Cast<ANpcAiController>(OwnerComp.GetAIOwner());	
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(!IsValid(AiController) || !IsValid(Blackboard))
		return EBTNodeResult::Aborted;

	if(!IsValid(AiController->GetCurrentPointOfInterest()))
		return EBTNodeResult::Failed;
	
	Blackboard->SetValueAsObject(RetActorKey.SelectedKeyName, AiController->GetCurrentPointOfInterest());
	return EBTNodeResult::Succeeded;
}
