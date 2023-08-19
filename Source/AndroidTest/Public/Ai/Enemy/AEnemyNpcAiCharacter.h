// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "AEnemyNpcAiCharacter.generated.h"

class UAIPerceptionComponent;

UCLASS()
class ANDROIDTEST_API AAEnemyNpcAiCharacter : public ANpcAiCharacter
{
	GENERATED_BODY()

public:
	AAEnemyNpcAiCharacter();
	
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UAIPerceptionComponent* PerceptionComponent;
};
