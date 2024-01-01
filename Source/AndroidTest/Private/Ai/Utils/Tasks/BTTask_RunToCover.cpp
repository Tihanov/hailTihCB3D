// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_RunToCover.h"

#include "Log.h"
#include "Ai/Enemy/NpcEnemyController.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "Ai/Npc/NpcMovementComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Navigation/PathFollowingComponent.h"
#include "Utils/Utils.h"

struct FTaskData_ShootingFromCover
{
	FEnvQueryRequest CoverFindingEqsRequest;
	ANpcEnemyController* NpcController;
};

UBTTask_RunToCover::UBTTask_RunToCover()
{
	NodeName = "Run To Cover";
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_RunToCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ANpcEnemyController>(OwnerComp.GetOwner());
	if(!Controller)
	{
		ULog::Error("UBTTask_ShootingFromCover is using only with ANpcEnemyController", LO_Both);
		return EBTNodeResult::Aborted;
	}
	if(!Controller->GetHostileActor())
		return EBTNodeResult::Failed;
	Controller->SetHostilePointFromHostileActor();

	/*
	 * TODO make is property
	 */
	Controller->SetSenseHostile(false);

	/*
	 * TODO move EquipWeapon to ShootFromCover
	 */
	const auto Character = Controller->GetPawn<ANpcAiCharacter>();
	CHECK_ON_TRUE_DO_TASK(!Character, return EBTNodeResult::Aborted);
	Character->EquipWeapon();
	const auto MovementComponent = Cast<UNpcMovementComponent>(Character->GetCharacterMovement());
	CHECK_ON_TRUE_DO_TASK(!MovementComponent, return EBTNodeResult::Aborted);
	MovementComponent->Run();

	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);
	TaskData->CoverFindingEqsRequest = FEnvQueryRequest{CoverFindingEqs, Controller};
	TaskData->NpcController = Controller;

	FQueryFinishedSignature QueryFinishedDelegate;
	QueryFinishedDelegate.BindUObject(this, &UBTTask_RunToCover::OnEqsFinishedCallback, NodeMemory);
	TaskData->CoverFindingEqsRequest.Execute(EEnvQueryRunMode::RandomBest5Pct, QueryFinishedDelegate);
	return EBTNodeResult::InProgress;
}

void UBTTask_RunToCover::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);
	if(!IsValid(TaskData->NpcController))
		return;

	TaskData->NpcController->SetSenseHostile(true);
	
	const auto MovComp = Cast<UNpcMovementComponent>(TaskData->NpcController->GetPawn()->GetMovementComponent());
	if(!IsValid(MovComp))
		return;
	MovComp->StopRunning();
}

uint16 UBTTask_RunToCover::GetInstanceMemorySize() const
{
	return sizeof(FTaskData_ShootingFromCover);
}

void UBTTask_RunToCover::OnEqsFinishedCallback(TSharedPtr<FEnvQueryResult> Result, uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);
	if(!Result->IsSuccessful())
	{
		FQueryFinishedSignature QueryFinishedDelegate;
		QueryFinishedDelegate.BindUObject(this, &UBTTask_RunToCover::OnEqsFinishedCallback, NodeMemory);
		TaskData->CoverFindingEqsRequest.Execute(EEnvQueryRunMode::RandomBest5Pct, QueryFinishedDelegate);
		return;
	}
	check(Result->Owner.IsValid());
	
	const auto Controller = Cast<ANpcEnemyController>(Result->Owner.Get());
	FAIMoveRequest MoveReq(Result->GetItemAsLocation(0));
	MoveReq.SetCanStrafe(false);
	Controller->MoveTo(MoveReq);
	Controller->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTTask_RunToCover::OnArrivedCallback,
		NodeMemory);
}

void UBTTask_RunToCover::OnArrivedCallback(FAIRequestID RequestID, const FPathFollowingResult& Result,
	uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);
	CHECK_ON_TRUE_JUST_RETURN(!IsValid(TaskData->NpcController));
	const auto BTComp = Cast<UBehaviorTreeComponent>(TaskData->NpcController->GetBrainComponent());
	check(BTComp);
	if(Result.IsSuccess())
		FinishLatentTask(*BTComp, Result.IsSuccess() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

