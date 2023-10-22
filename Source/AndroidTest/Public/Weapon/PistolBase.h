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

	virtual void PullTheTrigger() override;
	virtual void ReleaseTheTrigger() override;
	virtual bool CanBeUsedNow() const override;
	
	virtual TArray<AActor*> MakeTestAttack() override;
	
	virtual float GetScatter() const override;
	
	virtual bool CanBeReloaded() const override;
	virtual void Reload() override;
	virtual bool IsReloading() const override;
	virtual float GetCurrentReloadingTimeout() const override;
	virtual int GetMagazineCapacity() const override;
	
private:
	void ReduceScatter(float DeltaTime);
	void TryReload(float DeltaTime);
	void ShotDelay(float DeltaTime);
};

UCLASS(BlueprintType)
class UPistolInfo : public UFirearmsInfo { GENERATED_BODY() };
