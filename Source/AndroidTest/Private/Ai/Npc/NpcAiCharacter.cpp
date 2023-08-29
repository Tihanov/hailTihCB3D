// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Npc/NpcAiCharacter.h"

#include "Utils/UtilsStructs.h"
#include "Ai/Npc/NpcAiController.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


ANpcAiCharacter::ANpcAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		"PerceptionStimuliSourceComponent");
	PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	
	AIControllerClass = ANpcAiController::StaticClass();
	GetCapsuleComponent()->ComponentTags.Add(FName(TEXT("MainCapsule")));
}

FGenericTeamId ANpcAiCharacter::GetGenericTeamId() const
{
	return TI_DefaultNpc;
}

void ANpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();
}
