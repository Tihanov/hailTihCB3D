// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NpcUtils.generated.h"

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ENpcState : uint8
{
	NONE		= 0 UMETA(Hidden),
	// Can patrol from point to point of interest
	Patrol		= 1 << 0,
	// Can use weapon to shoot(Use only with NpcEnemyController)
	Aggressive	= 1 << 1,
	// Will run from aggressor(should not use with aggressive)
	Scared		= 1 << 2,
	// Should be use without other states 
	Custom		= 1 << 3,
	// Should be without behaviour tree
	Dead		= 1 << 4, 
};
ENUM_CLASS_FLAGS(ENpcState);
