// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryStructures.h"
#include "WeaponBase.generated.h"

USTRUCT(BlueprintType)
struct FDamagedActorsAndDamageProxyMap
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<AActor*, float> DamagedActors;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMadeShotDelegate,
	class AWeaponBase*, Weapon,
	bool, IsDamageWasDone,
	FDamagedActorsAndDamageProxyMap, DamagedActors);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartShootingDelegate,
	class AWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopShootingDelegate,
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
	UFUNCTION(BlueprintPure, Category = "Weapon")
		FInvItemDataTable GetWeaponSettings() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
		void InitAsEquippedWeapon(APawn* WeaponOwner, FInvItemDataTable Options, FName ItemName);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Shoot")
		void StartShooting();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Shoot")
		void StopShooting();
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Weapon")
		bool CanWeaponShoot() const;
	
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
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates", DisplayName = "OnStartShooting")
		FOnStartShootingDelegate OnStartShootingDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates", DisplayName = "OnStopShooting")
		FOnStopShootingDelegate OnStopShootingDelegate;
};
