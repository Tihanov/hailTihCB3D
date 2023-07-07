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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
		void InitAsEquippedWeapon(AActor* CausedBy, FInvItemDataTable Options);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
		void WeaponTickUpdate(float DeltaTime);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Shoot")
		void StartShooting();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Shoot")
		void StopShooting();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Scatter")
		float GetWeaponScatter() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Reload")
		FInvItemDataTable GetWeaponSettings() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Reload")
		void ReloadWeapon();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Reload")
		bool IsWeaponReloading() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Reload")
		float GetCurrentWeaponReloadingTimeout() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Reload")
		int GetMagazineCapacity() const;
};
