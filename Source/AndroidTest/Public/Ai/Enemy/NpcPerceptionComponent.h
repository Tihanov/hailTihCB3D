// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "NpcPerceptionComponent.generated.h"


class ANpcAiCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UNpcPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	UNpcPerceptionComponent();

	virtual void GetHostileActors(TArray<AActor*>& OutActors) const override;
};
