// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetNpcStats.generated.h"

UCLASS()
class ANDROIDTEST_API UBTService_GetNpcStats : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_GetNpcStats();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Output")
		FBlackboardKeySelector OutHostileActor;
	UPROPERTY(EditAnywhere, Category = "Output")
		FBlackboardKeySelector OutStress;
};
