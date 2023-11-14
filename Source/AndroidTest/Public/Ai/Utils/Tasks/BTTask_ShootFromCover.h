// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ShootFromCover.generated.h"

class ANpcAiCharacter;
class ANpcEnemyController;
class UAIShootComponent;
class UWeaponInfo;

UCLASS()
class ANDROIDTEST_API UBTTask_ShootFromCover : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ShootFromCover();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

private:
	UFUNCTION()
	void OnChangeShootingStateCallback(UAIShootComponent* ShootComponent);

	bool GetControllerAndCharacterFromContComponent(const UActorComponent* ActorComponent,
	                                                ANpcEnemyController*& OutNpcEnemyController,
	                                                ANpcAiCharacter*& OutNpcAiCharacter) const;
	UBehaviorTreeComponent* GetBTCompFromController(const AAIController* Controller) const;
};
