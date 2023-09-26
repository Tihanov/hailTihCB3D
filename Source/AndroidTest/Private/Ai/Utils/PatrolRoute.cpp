// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/PatrolRoute.h"


APatrolRoute::APatrolRoute()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

void APatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

