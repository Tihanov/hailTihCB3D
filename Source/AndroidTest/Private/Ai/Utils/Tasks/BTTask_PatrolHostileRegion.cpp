// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_PatrolHostileRegion.h"

#include "Log.h"
#include "NavigationSystem.h"
#include "Ai/Enemy/NpcEnemyController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Utils/Utils.h"

struct FTaskData_PatrolHostileRegion
{
	FVector LastPosition;
	ANpcEnemyController* EnemyController;
	UBehaviorTreeComponent* BTComp;

	int32 CurrentCountOfMoving = 0;

	FDelegateHandle PathFolReqFinisedDelegHandle;
	FTimerHandle StandDelayTimerHandle;
};

UBTTask_PatrolHostileRegion::UBTTask_PatrolHostileRegion()
{
	NodeName = "Patrol Hostile Region";
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_PatrolHostileRegion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_PatrolHostileRegion>(NodeMemory);
	CHECK_ON_TRUE_DO_TASK(TaskData == nullptr, return EBTNodeResult::Aborted);

	const auto AiController = OwnerComp.GetOwner<ANpcEnemyController>();
	CHECK_ON_TRUE_DO_TASK(AiController == nullptr, return EBTNodeResult::Aborted);

	TaskData->BTComp = &OwnerComp;
	TaskData->EnemyController = AiController;
	TaskData->LastPosition = AiController->GetHostilePoint();
	TaskData->CurrentCountOfMoving = 0;

	MoveToNextPoint(NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolHostileRegion::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                 EBTNodeResult::Type TaskResult)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_PatrolHostileRegion>(NodeMemory);
	if (!TaskData || !IsValid(TaskData->EnemyController))
		return;

	TaskData->EnemyController->GetPathFollowingComponent()->OnRequestFinished.Remove(
		TaskData->PathFolReqFinisedDelegHandle);

	GetWorld()->GetTimerManager().ClearTimer(TaskData->StandDelayTimerHandle);
}

uint16 UBTTask_PatrolHostileRegion::GetInstanceMemorySize() const
{
	return sizeof(FTaskData_PatrolHostileRegion);
}

void UBTTask_PatrolHostileRegion::MoveToNextPoint(uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_PatrolHostileRegion>(NodeMemory);
	if (!TaskData || !IsValid(TaskData->EnemyController) || !IsValid(TaskData->BTComp))
		return;

	if (TaskData->CurrentCountOfMoving >= MaxCountOfMovement)
	{
		FinishLatentTask(*TaskData->BTComp, ReturnType);
		return;
	}

	const auto NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	check(NavSystem);

	FNavLocation NavLocation;
	if (!NavSystem->GetRandomReachablePointInRadius(TaskData->LastPosition, PatrolRadius, NavLocation))
	{
		FinishLatentTask(*TaskData->BTComp, EBTNodeResult::Aborted);
		return;
	}

	FNavPathSharedPtr Path;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetCanStrafe(false);
	MoveRequest.SetGoalLocation(NavLocation.Location);
	const auto PathFollowingResult = TaskData->EnemyController->MoveTo(MoveRequest, &Path);
	if (PathFollowingResult.Code != EPathFollowingRequestResult::RequestSuccessful)
		MoveToNextPoint(NodeMemory);

	TaskData->CurrentCountOfMoving += 1;

	TaskData->PathFolReqFinisedDelegHandle =
		TaskData->EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddWeakLambda(this,
			[this, PathFollowingResult, NodeMemory](FAIRequestID RequestID, const FPathFollowingResult& Result)
			{
				if (PathFollowingResult.MoveId != RequestID)
					return;

				const auto TaskData = CastInstanceNodeMemory<FTaskData_PatrolHostileRegion>(NodeMemory);
				if (!TaskData || !IsValid(TaskData->EnemyController))
					return;
				
				TaskData->EnemyController->GetPathFollowingComponent()->OnRequestFinished.Remove(TaskData->PathFolReqFinisedDelegHandle);

				// Sets standing timer and call MoveToNextPoint after end
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &UBTTask_PatrolHostileRegion::MoveToNextPoint, NodeMemory);
				GetWorld()->GetTimerManager().SetTimer(TaskData->StandDelayTimerHandle, Delegate,
				                                       FMath::RandRange(StandingTime - StandingTimeRandomDeviation,
				                                                        StandingTime + StandingTimeRandomDeviation),
				                                       false);
			}
		);
}
