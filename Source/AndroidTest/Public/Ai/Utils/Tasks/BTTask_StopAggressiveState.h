// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Utils/NpcUtils.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopAggressiveState.generated.h"

UCLASS()
class ANDROIDTEST_API UBTTask_StopAggressiveState : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_StopAggressiveState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Options")
		ENpcState NewState;
};
