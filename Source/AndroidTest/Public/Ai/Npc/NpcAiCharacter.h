// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Npc/NpcBaseCharacter.h"
#include "NpcAiCharacter.generated.h"


class UBehaviorTree;
class UAiPointOfInterestInstance;

UCLASS(BlueprintType, Blueprintable)
class ANDROIDTEST_API ANpcAiCharacter : public ANpcBaseCharacter
{
	GENERATED_BODY()

public:
	ANpcAiCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ai")
		UBehaviorTree* StartupBehaviorTree;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Ai|Point Of Interest")
		TArray<UAiPointOfInterestInstance*> PointsOfInterest;
};