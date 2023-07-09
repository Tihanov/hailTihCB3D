// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"


AMainGameState::AMainGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

