// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BTTask_RunToCover.generated.h"

class UEnvQuery;
struct FPathFollowingResult;

UCLASS()
class ANDROIDTEST_API UBTTask_RunToCover : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RunToCover();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Options")
		UEnvQuery* CoverFindingEqs;

private:
	void OnEqsFinishedCallback(TSharedPtr<FEnvQueryResult> Result, uint8* NodeMemory);
	void OnArrivedCallback(FAIRequestID RequestID, const FPathFollowingResult& Result, uint8* NodeMemory);
};
