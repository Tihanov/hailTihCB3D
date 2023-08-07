// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Tasks/AITask.h"
#include "BTTask_GetCurrentPointOfInterest.generated.h"

UCLASS()
class ANDROIDTEST_API UBTTask_GetCurrentPointOfInterest : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetCurrentPointOfInterest();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options", DisplayName = "Actor Key")
		FBlackboardKeySelector RetActorKey;
};
