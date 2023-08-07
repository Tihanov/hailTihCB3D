// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Villager/VillagerCharacter.h"

#include "Ai/Villager/VillagerAiController.h"


AVillagerCharacter::AVillagerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = AVillagerAiController::StaticClass();
}

void AVillagerCharacter::BeginPlay()
{
	Super::BeginPlay();

}
