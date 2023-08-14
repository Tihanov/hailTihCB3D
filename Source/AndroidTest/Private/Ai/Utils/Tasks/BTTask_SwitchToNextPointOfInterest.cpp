// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_SwitchToNextPointOfInterest.h"

#include "Ai/Npc/NpcAiController.h"

UBTTask_SwitchToNextPointOfInterest::UBTTask_SwitchToNextPointOfInterest()
{
	NodeName = "Switch To Next Point Of Interest";
}

EBTNodeResult::Type UBTTask_SwitchToNextPointOfInterest::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	const auto AiController = Cast<ANpcAiController>(OwnerComp.GetAIOwner());
	if(!IsValid(AiController))
		return EBTNodeResult::Aborted;

	AiController->SwapToNextInterest();
	return EBTNodeResult::Succeeded;
}
