// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/Npc/NpcMovementComponent.h"

UNpcMovementComponent::UNpcMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UNpcMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxWalkingSpeedTemp = MaxWalkSpeed;
}

void UNpcMovementComponent::Run()
{
	MaxWalkingSpeedTemp = MaxWalkSpeed;
	MaxWalkSpeed = MaxRunningSpeed;
}

void UNpcMovementComponent::StopRunning()
{
	MaxWalkSpeed = MaxWalkingSpeedTemp;
}

