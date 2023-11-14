// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/Utils/Tasks/BTTask_ShootFromCover.h"

#include "Log.h"
#include "Ai/Enemy/NpcEnemyController.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "Ai/Utils/Components/AIShootComponent.h"
#include "Utils/Utils.h"

struct FTaskData_ShootFromCover
{
};

UBTTask_ShootFromCover::UBTTask_ShootFromCover()
{
	NodeName = "Shoot From Cover";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ShootFromCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootFromCover>(NodeMemory);
	CHECK_RETURN(TaskData == nullptr, return EBTNodeResult::Aborted);

	ANpcEnemyController* EnemyController;
	ANpcAiCharacter* NpcCharacter;
	if(!GetControllerAndCharacterFromContComponent(&OwnerComp, EnemyController, NpcCharacter))
		return EBTNodeResult::Aborted;
	
	const auto AIShootComponent = EnemyController->GetAIShootComponent();
	CHECK_RETURN(AIShootComponent == nullptr, return EBTNodeResult::Aborted);
	AIShootComponent->OnChangeShootingStateDelegate.AddDynamic(this, &UBTTask_ShootFromCover::OnChangeShootingStateCallback);
	AIShootComponent->StartShooting();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_ShootFromCover::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto TaskData = CastInstanceNodeMemory<FTaskData_ShootFromCover>(NodeMemory);
	CHECK_RETURN(TaskData == nullptr, FinishLatentTask(OwnerComp, EBTNodeResult::Aborted));

	
}

uint16 UBTTask_ShootFromCover::GetInstanceMemorySize() const
{
	return sizeof(FTaskData_ShootFromCover);
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
	ANpcEnemyController*& OutNpcEnemyController, ANpcAiCharacter*& OutNpcAiCharacter) const
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
