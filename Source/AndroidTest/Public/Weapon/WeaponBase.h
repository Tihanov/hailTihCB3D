// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryStructures.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMadeShotDelegate,
	class AWeaponBase*, Weapon,
	bool, IsDamageWasDone,
	AActor*, DamagedActor,
	float, Damage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartShooting,
	class AWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopShooting,
	class AWeaponBase*, Weapon);

UCLASS(Abstract, BlueprintType, Blueprintable)
class ANDROIDTEST_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* RootMeshComponent;
	class UCameraComponent* OwnerAimCameraRight;
	FInvItemDataTable ItemSettings;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
		void InitAsEquippedWeapon(APawn* WeaponOwner, FInvItemDataTable Options, FName ItemName);
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Weapon")
		FInvItemDataTable GetWeaponSettings() const;
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Weapon")
		bool CanWeaponShoot() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Shoot")
		void StartShooting();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Shoot")
		void StopShooting();
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Weapon|Scatter")
		float GetWeaponScatter() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Reload")
		void ReloadWeapon();
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Weapon|Reload")
		bool IsWeaponReloading() const;
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Weapon|Reload")
		float GetCurrentWeaponReloadingTimeout() const;
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Weapon|Reload")
		int GetMagazineCapacity() const;

public: /*DELEGATES*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates", DisplayName = "OnMadeShot")
		FOnMadeShotDelegate OnMadeShotDelegate;
};
