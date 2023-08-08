// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_TryToDoneTaskInPoi.h"

#include "Log.h"
#include "Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TryToDoneTaskInPoi::UBTTask_TryToDoneTaskInPoi()
{
	NodeName = "Try To Done Task In Interest Point";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TryToDoneTaskInPoi::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(!IsValid(Blackboard))
		return EBTNodeResult::Aborted;

	const auto PointOfInterest_ = Cast<AAiPointOfInterest>(Blackboard->GetValueAsObject(PointOfInterestKey.SelectedKeyName));
	if(!IsValid(PointOfInterest_))
	{
		ULog::Error("PointOfInterestKey should be derived from AAiPointOfInterest", LO_Both);
		return EBTNodeResult::Aborted;
	}

	PointOfInterest = PointOfInterest_;
	return EBTNodeResult::InProgress;
}

void UBTTask_TryToDoneTaskInPoi::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(PointOfInterest.IsNull())
		Super::FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);

	if(PointOfInterest->IsComplete())
		Super::FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_TryToDoneTaskInPoi::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	PointOfInterest.Reset();
}
