// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Ai/Utils/NpcUtils.h"
#include "BTDecorator_CheckNpcState.generated.h"


UCLASS()
class ANDROIDTEST_API UBTDecorator_CheckNpcState : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckNpcState();
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		ENpcState State;
};
