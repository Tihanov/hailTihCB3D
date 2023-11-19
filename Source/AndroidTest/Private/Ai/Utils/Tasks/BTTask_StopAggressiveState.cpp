// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/Utils/Tasks/BTTask_StopAggressiveState.h"

#include "Log.h"
#include "Ai/Enemy/NpcEnemyController.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "Ai/Utils/Components/AIShootComponent.h"
#include "Utils/Utils.h"

UBTTask_StopAggressiveState::UBTTask_StopAggressiveState()
{
	NodeName = "Stop Aggressive State";
}

EBTNodeResult::Type UBTTask_StopAggressiveState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ANpcEnemyController>(OwnerComp.GetOwner());
	if(!Controller)
	{
		ULog::Error("UBTTask_ShootingFromCover is using only with ANpcEnemyController", LO_Both);
		return EBTNodeResult::Aborted;
	}

	Controller->ClearHostileActor();
	Controller->SetFreezeStressProgress(false);
	
	const auto Character = Controller->GetPawn<ANpcAiCharacter>();
	CHECK_RETURN(!Character, return EBTNodeResult::Aborted);
	Character->UnequipWeapon();
	Character->SetCurrentState(NewState);

	return EBTNodeResult::Succeeded;
}
