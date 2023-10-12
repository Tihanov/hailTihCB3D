// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon\NpcWeapon\NpcPistolBase.h"


// Sets default values
ANpcPistolBase::ANpcPistolBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANpcPistolBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANpcPistolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

