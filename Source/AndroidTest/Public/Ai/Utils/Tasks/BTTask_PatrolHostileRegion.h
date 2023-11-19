// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolHostileRegion.generated.h"

UCLASS()
class ANDROIDTEST_API UBTTask_PatrolHostileRegion : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PatrolHostileRegion();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
	virtual uint16 GetInstanceMemorySize() const override;

	void MoveToNextPoint(uint8* NodeMemory);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Options")
		TEnumAsByte<EBTNodeResult::Type> ReturnType = EBTNodeResult::Failed;
	UPROPERTY(EditAnywhere, Category = "Options")
		int32 MaxCountOfMovement = 5;
	UPROPERTY(EditAnywhere, Category = "Options")
		float PatrolRadius = 300.f;
	UPROPERTY(EditAnywhere, Category = "Options")
		float StandingTime = 5.f;
	UPROPERTY(EditAnywhere, Category = "Options")
		float StandingTimeRandomDeviation = 1.f;
};
