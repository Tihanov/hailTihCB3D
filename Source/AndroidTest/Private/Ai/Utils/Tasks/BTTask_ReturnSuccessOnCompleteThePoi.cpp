// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_ReturnSuccessOnCompleteThePoi.h"

#include "Log.h"
#include "Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "BehaviorTree/BlackboardComponent.h"

struct FTaskData_ReturnSuccessOnCompleteThePoi
{
	TSoftObjectPtr<AAiPointOfInterest> PointOfInterest;
};

UBTTask_ReturnSuccessOnCompleteThePoi::UBTTask_ReturnSuccessOnCompleteThePoi()
{
	NodeName = "Return Success On Complete The Poi";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	
	PointOfInterestKey.AddObjectFilter(
		this,
		GET_MEMBER_NAME_CHECKED(UBTTask_ReturnSuccessOnCompleteThePoi, PointOfInterestKey),
		AAiPointOfInterest::StaticClass());
}

EBTNodeResult::Type UBTTask_ReturnSuccessOnCompleteThePoi::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	const auto Data = CastInstanceNodeMemory<FTaskData_ReturnSuccessOnCompleteThePoi>(NodeMemory);
	Data->PointOfInterest = PointOfInterest_;
	return EBTNodeResult::InProgress;
}

void UBTTask_ReturnSuccessOnCompleteThePoi::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Data = CastInstanceNodeMemory<FTaskData_ReturnSuccessOnCompleteThePoi>(NodeMemory);
	if(!Data->PointOfInterest.IsValid())
		Super::FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);

	if(Data->PointOfInterest->IsComplete())
		Super::FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_ReturnSuccessOnCompleteThePoi::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	const auto Data = CastInstanceNodeMemory<FTaskData_ReturnSuccessOnCompleteThePoi>(NodeMemory);
	Data->PointOfInterest.Reset();
}

uint16 UBTTask_ReturnSuccessOnCompleteThePoi::GetInstanceMemorySize() const
{
	return sizeof(FTaskData_ReturnSuccessOnCompleteThePoi);
}
