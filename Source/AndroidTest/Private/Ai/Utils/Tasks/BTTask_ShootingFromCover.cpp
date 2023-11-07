// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Tasks/BTTask_ShootingFromCover.h"

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
	bool bArrivedToCover;
	ANpcEnemyController* NpcController;
};

UBTTask_ShootingFromCover::UBTTask_ShootingFromCover()
{
	NodeName = "Shooting From Cover";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ShootingFromCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = Cast<ANpcEnemyController>(OwnerComp.GetOwner());
	if(!Controller)
	{
		ULog::Error("UBTTask_ShootingFromCover is using only with ANpcEnemyController", LO_Both);
		return EBTNodeResult::Aborted;
	}
	if(!Controller->GetTargetActor())
		return EBTNodeResult::Failed;
	Controller->SetFocus(Controller->GetTargetActor());
	Controller->SetFreezeStressProgress(true);
	
	const auto Character = Controller->GetPawn<ANpcAiCharacter>();
	CHECK_RETURN(!Character, return EBTNodeResult::Aborted);
	Character->EquipWeapon();
	Character->StartShooting();
	const auto MovementComponent = Cast<UNpcMovementComponent>(Character->GetCharacterMovement());
	CHECK_RETURN(!MovementComponent, return EBTNodeResult::Aborted);
	MovementComponent->Run();

	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);
	TaskData->CoverFindingEqsRequest = FEnvQueryRequest{CoverFindingEqs, Controller};
	TaskData->bArrivedToCover = false;
	TaskData->NpcController = Controller;

	FQueryFinishedSignature QueryFinishedDelegate;
	QueryFinishedDelegate.BindUObject(this, &UBTTask_ShootingFromCover::OnEqsFinishedCallback, NodeMemory);
	TaskData->CoverFindingEqsRequest.Execute(EEnvQueryRunMode::RandomBest5Pct, QueryFinishedDelegate);
	return EBTNodeResult::InProgress;
}

void UBTTask_ShootingFromCover::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);

	if(TaskData->bArrivedToCover)
		ULog::Wassup(LO_Both);
}

uint16 UBTTask_ShootingFromCover::GetInstanceMemorySize() const
{
	return sizeof(FTaskData_ShootingFromCover);
}

void UBTTask_ShootingFromCover::OnEqsFinishedCallback(TSharedPtr<FEnvQueryResult> Result, uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);
	if(!Result->IsSuccessful())
	{
		FQueryFinishedSignature QueryFinishedDelegate;
		QueryFinishedDelegate.BindUObject(this, &UBTTask_ShootingFromCover::OnEqsFinishedCallback, NodeMemory);
		TaskData->CoverFindingEqsRequest.Execute(EEnvQueryRunMode::RandomBest5Pct, QueryFinishedDelegate);
		return;
	}
	check(Result->Owner.IsValid());
	
	const auto Controller = Cast<ANpcEnemyController>(Result->Owner.Get());
	const FAIMoveRequest MoveReq(Result->GetItemAsLocation(0));
	Controller->MoveTo(MoveReq);
	Controller->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTTask_ShootingFromCover::OnArrivedCallback,
		NodeMemory);
}

void UBTTask_ShootingFromCover::OnArrivedCallback(FAIRequestID RequestID, const FPathFollowingResult& Result,
	uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootingFromCover>(NodeMemory);
	check(TaskData);
	TaskData->bArrivedToCover = Result.IsSuccess();
	CHECK_RETURN_ON_FAIL(!IsValid(TaskData->NpcController));
}

