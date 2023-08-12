// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Npc/NpcCharacter.h"
#include "VillagerCharacter.generated.h"


class UBehaviorTree;
class UAiPointOfInterestInstance;

UCLASS(BlueprintType, Blueprintable)
class ANDROIDTEST_API AVillagerCharacter : public ANpcCharacter
{
	GENERATED_BODY()

public:
	AVillagerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ai")
		UBehaviorTree* StartupBehaviorTree;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Ai|Point Of Interest")
		TArray<UAiPointOfInterestInstance*> PointsOfInterest;
};
