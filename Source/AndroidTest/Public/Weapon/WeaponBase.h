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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttackDelegate,
	class AWeaponBase*, Weapon,
	bool, IsDamageWasDone,
	FDamagedActorsAndDamageProxyMap, DamagedActors);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPullTheTriggerDelegate,
	class AWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReleaseTheTriggerDelegate,
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

	UFUNCTION(BlueprintPure, Category = "Weapon")
		FInvItemDataTable GetWeaponSettings() const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void PullTheTrigger();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void ReleaseTheTrigger();
	UFUNCTION(BlueprintPure, Category = "Weapon", DisplayName = "Can Be Used Now?")
		virtual bool CanBeUsedNow() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual TArray<AActor*> MakeTestAttack();
	
	UFUNCTION(BlueprintPure, Category = "Weapon")
		virtual float GetScatter() const;

	UFUNCTION(BlueprintPure, Category = "Weapon|Reload")
		virtual bool CanBeReloaded() const;
	UFUNCTION(BlueprintCallable, Category = "Weapon|Reload")
		virtual void Reload();
	UFUNCTION(BlueprintPure, Category = "Weapon|Reload")
		virtual bool IsReloading() const;
	UFUNCTION(BlueprintPure, Category = "Weapon|Reload")
		virtual float GetCurrentReloadingTimeout() const;
	UFUNCTION(BlueprintPure, Category = "Weapon|Reload")
		virtual int GetMagazineCapacity() const;

	UFUNCTION(BlueprintPure, Category = "Weapon") const class UWeaponInfo* GetInfo() const { return ItemSettings.Other.WeaponInfo; }
	template<class T> const T* GetInfo() const { return Cast<T>(ItemSettings.Other.WeaponInfo); }
	
public: /*DELEGATES*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates", DisplayName = "OnAttack")
		FOnAttackDelegate OnAttackDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates", DisplayName = "OnPullTheTrigger")
		FOnPullTheTriggerDelegate OnPullTheTriggerDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates", DisplayName = "OnReleaseTheTrigger")
		FOnReleaseTheTriggerDelegate OnReleaseTheTriggerDelegate;
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UWeaponInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)				TSubclassOf<class AWeaponBase>		WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 			class UAnimMontage* 				AimAnimationMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)				class USoundBase*					ShotSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)				float								Damage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 			float								ShotDelayInSec = 1.f;

	/*RECOIL START*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil") float						VerticalRecoilInShot = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil") UCurveFloat*				VerticalRecoilCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil") float						HorizontalRecoilInShot = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil") UCurveFloat*				HorizontalRecoilCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil") float						RecoilTimeRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil") float						RecoilTimeLenghtInSec = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil") bool						StopRecoilImmediatelyAfterStopShooting = false;
	/*RECOIL END*/
};

UCLASS(BlueprintType, Abstract)
class UFirearmsInfo : public UWeaponInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int												MagazineCapacity = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float											ShotRange = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float											ReloadTimeoutInSec = 1.f;
								
	/*SCATTER*/							
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scatter") float						ShotScatter = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scatter") float						MaxScatter = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scatter") float						MinScatter = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scatter") float						ScatterReductionInOneSec = 10.f;
	/*SCATTER*/
};
