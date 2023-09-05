// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Decoratos/BTDecorator_CheckNpcState.h"

#include "Log.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "Ai/Npc/NpcAiController.h"

UBTDecorator_CheckNpcState::UBTDecorator_CheckNpcState()
{
	State = ENpcState::Custom;
	NodeName = TEXT("Check Node State");
}

FString UBTDecorator_CheckNpcState::GetStaticDescription() const
{
	return FString::Printf(TEXT("Is NpcState == %s"), *UEnum::GetValueAsString(State));
}

bool UBTDecorator_CheckNpcState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto NpcCharacter = OwnerComp.GetAIOwner()->GetPawn<ANpcAiCharacter>();
	if(!NpcCharacter)
	{
		ULog::Warning("UBTDecorator_CheckNpcState: NpcCharacter == nullptr", LO_Both);
		return false;
	}

	return NpcCharacter->GetCurrentState() == State;
}

void UBTDecorator_CheckNpcState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto NpcCharacter = OwnerComp.GetAIOwner()->GetPawn<ANpcAiCharacter>();
	if(!NpcCharacter)
	{
		ULog::Warning("UBTDecorator_CheckNpcState: NpcCharacter == nullptr", LO_Both);
		ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionPassing);
	}

	if(NpcCharacter->GetCurrentState() == State)
		ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionPassing);
}
