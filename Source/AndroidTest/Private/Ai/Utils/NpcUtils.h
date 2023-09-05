// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NpcUtils.generated.h"

UENUM(BlueprintType)
enum class ENpcState : uint8
{
	// Can patrol from point to point of interest
	Patrol,
	// Can use weapon to shoot(Use only with NpcEnemyController)
	Aggressive,
	// Will run from aggressor(should not use with aggressive)
	Scared,
	// Should be use without other states 
	Custom,
	// Should be without behaviour tree
	Dead, 
};
