// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Service/BTService_GetNpcStats.h"

#include "Log.h"
#include "Ai/Enemy/NpcEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/Utils.h"

UBTService_GetNpcStats::UBTService_GetNpcStats()
{
	NodeName = "Get Npc Stats";
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_GetNpcStats::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const auto EnemyController = OwnerComp.GetOwner<ANpcEnemyController>();
	CHECK_RETURN_ON_FAIL(EnemyController == nullptr);
	EnemyController->OnHostileActorUpdateDelegate.AddLambda(
		[this](ANpcEnemyController* EnemyNpcController, AActor* HostileActor, FVector FocalPoint,
		       UBehaviorTreeComponent* OwnerComp)
		{
			const auto BBComp = OwnerComp->GetBlackboardComponent();
			check(BBComp);
			BBComp->SetValueAsObject(OutHostileActor.SelectedKeyName, HostileActor);
		}, &OwnerComp);

	const auto BBComp = OwnerComp.GetBlackboardComponent();
	check(BBComp);
	BBComp->SetValueAsObject(OutHostileActor.SelectedKeyName, EnemyController->GetHostileActor());
	BBComp->SetValueAsFloat(OutStress.SelectedKeyName, EnemyController->GetStressProgress());
}

void UBTService_GetNpcStats::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto EnemyController = OwnerComp.GetOwner<ANpcEnemyController>();
	CHECK_RETURN_ON_FAIL(EnemyController == nullptr);

	if(EnemyController->IsFreezeStressProgress())
		return;
	const auto BBComp = OwnerComp.GetBlackboardComponent();
	check(BBComp);
	BBComp->SetValueAsFloat(OutStress.SelectedKeyName, EnemyController->GetStressProgress());
}
