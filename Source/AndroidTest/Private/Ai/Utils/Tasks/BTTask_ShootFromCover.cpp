// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/Utils/Tasks/BTTask_ShootFromCover.h"

#include "Log.h"
#include "Ai/Enemy/NpcEnemyController.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "Ai/Utils/Components/AIShootComponent.h"
#include "Utils/Utils.h"

struct FTaskData_ShootFromCover
{
	UBehaviorTreeComponent* BTComponent;
};

UBTTask_ShootFromCover::UBTTask_ShootFromCover()
{
	NodeName = "Shoot From Cover";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_ShootFromCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootFromCover>(NodeMemory);
	CHECK_RETURN(TaskData == nullptr, return EBTNodeResult::Aborted);
	TaskData->BTComponent = &OwnerComp;

	ANpcEnemyController* EnemyController;
	ANpcAiCharacter* NpcCharacter;
	if (!GetControllerAndCharacterFromContComponent(&OwnerComp, EnemyController, NpcCharacter))
		return EBTNodeResult::Aborted;

	const auto AIShootComponent = EnemyController->GetAIShootComponent();
	CHECK_RETURN(AIShootComponent == nullptr, return EBTNodeResult::Aborted);
	AIShootComponent->OnChangeShootingStateDelegate.AddUObject(
		this, &UBTTask_ShootFromCover::OnChangeShootingStateCallback);
	AIShootComponent->StartShooting();

	return EBTNodeResult::InProgress;
}

void UBTTask_ShootFromCover::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ANpcEnemyController* EnemyController;
	ANpcAiCharacter* NpcCharacter;
	if (!GetControllerAndCharacterFromContComponent(&OwnerComp, EnemyController, NpcCharacter))
		return FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	const auto HostileActor = EnemyController->GetHostileActor();

	// Stop forgeting
	if (HostileActor && GetWorld()->GetTimerManager().IsTimerActive(ForgetTimerHandle)
		&& EnemyController->GetAIShootComponent()->GetShootingState() == EShootingState::Shooting)
	{
		GetWorld()->GetTimerManager().ClearTimer(ForgetTimerHandle);
		return;
	}

	// Start forgeting
	if (!HostileActor && !GetWorld()->GetTimerManager().IsTimerActive(ForgetTimerHandle)
		&& EnemyController->GetAIShootComponent()->GetShootingState() == EShootingState::Shooting)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UBTTask_ShootFromCover::OnForgetCallback, NodeMemory);

		GetWorld()->GetTimerManager().SetTimer(ForgetTimerHandle, Delegate, ForgetTimeSec, false);
	}
}

uint16 UBTTask_ShootFromCover::GetInstanceMemorySize() const
{
	return sizeof(FTaskData_ShootFromCover);
}

void UBTTask_ShootFromCover::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	if(TaskResult == EBTNodeResult::Aborted)
		return;
	
	ANpcEnemyController* EnemyController;
	ANpcAiCharacter* NpcCharacter;
	if (!GetControllerAndCharacterFromContComponent(&OwnerComp, EnemyController, NpcCharacter))
		return;

	EnemyController->GetAIShootComponent()->StopShooting();
}

void UBTTask_ShootFromCover::OnChangeShootingStateCallback(UAIShootComponent* ShootComponent)
{
	switch (ShootComponent->GetShootingState())
	{
	case EShootingState::None:
		{
			ANpcEnemyController* EnemyController;
			ANpcAiCharacter* NpcCharacter;
			check(GetControllerAndCharacterFromContComponent(ShootComponent, EnemyController, NpcCharacter));
			NpcCharacter->UnCrouch();
			break;
		}

	case EShootingState::Shooting:
		{
			ANpcEnemyController* EnemyController;
			ANpcAiCharacter* NpcCharacter;
			check(GetControllerAndCharacterFromContComponent(ShootComponent, EnemyController, NpcCharacter));
			NpcCharacter->UnCrouch();
			break;
		}

	case EShootingState::Reloading:
		{
			ANpcEnemyController* EnemyController;
			ANpcAiCharacter* NpcCharacter;
			check(GetControllerAndCharacterFromContComponent(ShootComponent, EnemyController, NpcCharacter));
			NpcCharacter->Crouch();
			break;
		}
	}
}

bool UBTTask_ShootFromCover::GetControllerAndCharacterFromContComponent(const UActorComponent* ActorComponent,
                                                                        ANpcEnemyController*& OutNpcEnemyController,
                                                                        ANpcAiCharacter*& OutNpcAiCharacter) const
{
	OutNpcEnemyController = ActorComponent->GetOwner<ANpcEnemyController>();
	CHECK_RETURN(OutNpcEnemyController == nullptr, return false);

	OutNpcAiCharacter = OutNpcEnemyController->GetPawn<ANpcAiCharacter>();
	CHECK_RETURN(OutNpcAiCharacter == nullptr, return false);
	return true;
}

UBehaviorTreeComponent* UBTTask_ShootFromCover::GetBTCompFromController(const AAIController* Controller) const
{
	const auto BTComp = Cast<UBehaviorTreeComponent>(Controller->GetBrainComponent());
	check(BTComp);
	return BTComp;
}

void UBTTask_ShootFromCover::OnForgetCallback(uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootFromCover>(NodeMemory);
	CHECK_RETURN_ON_FAIL(TaskData == nullptr);
	CHECK_RETURN_ON_FAIL(TaskData->BTComponent == nullptr);

	FinishLatentTask(*TaskData->BTComponent, EBTNodeResult::Failed);
}
