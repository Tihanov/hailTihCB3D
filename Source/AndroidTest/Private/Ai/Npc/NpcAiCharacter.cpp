// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Npc/NpcAiCharacter.h"

#include "Ai/Npc/NpcAiController.h"


ANpcAiCharacter::ANpcAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = ANpcAiController::StaticClass();
}

void ANpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();
}
