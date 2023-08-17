// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_ReturnSuccessOnArrivedToPoi.h"

#include "Log.h"
#include "Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "BehaviorTree/BlackboardComponent.h"

struct FTaskData_ReturnSuccessOnArrivedToPoi
{
	TSoftObjectPtr<AAiPointOfInterest> PointOfInterest;
};

UBTTask_ReturnSuccessOnArrivedToPoi::UBTTask_ReturnSuccessOnArrivedToPoi()
{
	NodeName = "Return Success On Arrived To Poi";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	
	PointOfInterestKey.AddObjectFilter(
		this,
		GET_MEMBER_NAME_CHECKED(UBTTask_ReturnSuccessOnArrivedToPoi, PointOfInterestKey),
		AAiPointOfInterest::StaticClass());
}

EBTNodeResult::Type UBTTask_ReturnSuccessOnArrivedToPoi::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(!IsValid(Blackboard))
		return EBTNodeResult::Aborted;

	const auto PointOfInterest_ = Cast<AAiPointOfInterest>(Blackboard->GetValueAsObject(PointOfInterestKey.SelectedKeyName));
	if(!IsValid(PointOfInterest_))
	{
		ULog::Error("PointOfInterestKey should be derived from AAiPointOfInterest", LO_Both);
		return EBTNodeResult::Aborted;
	}

	const auto Data = CastInstanceNodeMemory<FTaskData_ReturnSuccessOnArrivedToPoi>(NodeMemory);
	Data->PointOfInterest = PointOfInterest_;
	return EBTNodeResult::InProgress;
}

void UBTTask_ReturnSuccessOnArrivedToPoi::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	const auto Data = CastInstanceNodeMemory<FTaskData_ReturnSuccessOnArrivedToPoi>(NodeMemory);
	if(!Data->PointOfInterest.IsValid())
		Super::FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);

	if(Data->PointOfInterest->IsArrived())
		Super::FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_ReturnSuccessOnArrivedToPoi::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	const auto Data = CastInstanceNodeMemory<FTaskData_ReturnSuccessOnArrivedToPoi>(NodeMemory);
	Data->PointOfInterest.Reset();
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

uint16 UBTTask_ReturnSuccessOnArrivedToPoi::GetInstanceMemorySize() const
{
	return sizeof(FTaskData_ReturnSuccessOnArrivedToPoi);	
}
