// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/AEnemyNpcAiCharacter.h"

#include "Utils/UtilsStructs.h"
#include "Perception/AIPerceptionComponent.h"

AAEnemyNpcAiCharacter::AAEnemyNpcAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
}

FGenericTeamId AAEnemyNpcAiCharacter::GetGenericTeamId() const
{
	return TI_DefaultEnemy;
}

void AAEnemyNpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
