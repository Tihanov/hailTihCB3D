// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Npc/NpcAiCharacter.h"

#include "Ai/Npc/NpcAiController.h"
#include "Components/CapsuleComponent.h"


ANpcAiCharacter::ANpcAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = ANpcAiController::StaticClass();
	GetCapsuleComponent()->ComponentTags.Add(FName(TEXT("MainCapsule")));
}

void ANpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();
}
