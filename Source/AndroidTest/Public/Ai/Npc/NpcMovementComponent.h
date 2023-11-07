// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NpcMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UNpcMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UNpcMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Run();
	void StopRunning();

protected:
	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
		float MaxRunningSpeed = 600.f;

private:
	float MaxWalkingSpeedTemp;
};
