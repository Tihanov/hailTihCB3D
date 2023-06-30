// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory/InventoryStructures.h"
#include "InvWeaponItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInvWeaponItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANDROIDTEST_API IInvWeaponItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InvWeaponInterface")
		void InitAsEquippedWeapon(FInvItemDataTable Options);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InvWeaponInterface")
		void Shoot(AActor* CausedBy);
};
