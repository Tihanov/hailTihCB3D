// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryToDoneTaskInPoi.generated.h"

class AAiPointOfInterest;

UCLASS()
class ANDROIDTEST_API UBTTask_TryToDoneTaskInPoi : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TryToDoneTaskInPoi();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Options")
		FBlackboardKeySelector PointOfInterestKey;

	TSoftObjectPtr<AAiPointOfInterest> PointOfInterest;
};
