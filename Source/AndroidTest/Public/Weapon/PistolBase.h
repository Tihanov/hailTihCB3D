// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "PistolBase.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ANDROIDTEST_API APistolBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	APistolBase();

protected:
	float CurrentScatter			= 0.f;
	
	int CurrentMagazineCapacity		= 0;
	bool IsWeaponInReloading		= false;
	float CurrentReloadTimeout		= 0.f;

	bool IsShotDelay				= false;
	float CurrentShotDelay			= 0.f;
	
public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void InitAsEquippedWeapon_Implementation(APawn* WeaponOwner, FInvItemDataTable Options, FName ItemName) override;
	
	virtual void StartShooting_Implementation() override;
	virtual void StopShooting_Implementation() override;
	virtual bool CanWeaponShoot_Implementation() const override;

	virtual TArray<AActor*> MakeTestShoot_Implementation() override;
	
	virtual float GetWeaponScatter_Implementation() const override;

	virtual void ReloadWeapon_Implementation() override;
	virtual bool IsWeaponReloading_Implementation() const override;
	virtual float GetCurrentWeaponReloadingTimeout_Implementation() const override;
	virtual int GetMagazineCapacity_Implementation() const override;

private:
	void ReduceScatter(float DeltaTime);
	void TryReload(float DeltaTime);
	void ShotDelay(float DeltaTime);
};
