// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SwitchToNextPointOfInterest.generated.h"

UCLASS()
class ANDROIDTEST_API UBTTask_SwitchToNextPointOfInterest : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SwitchToNextPointOfInterest();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
