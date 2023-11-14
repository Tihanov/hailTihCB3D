// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponBase.h"
#include "AIShootComponent.generated.h"

class ANpcAiController;
class ANpcAiCharacter;
class UAIShootComponent;

UENUM(BlueprintType)
enum class EShootingState : uint8
{
	None, Shooting, Reloading 
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeShootingStateDelegate,
	UAIShootComponent*, ShootComponent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UAIShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAIShootComponent();

public:
	void StartShooting();
	void StopShooting();

	EShootingState GetShootingState() const { return ShootingState; }
	void SetShootingState(EShootingState NewShootingState);

protected:
	EWeaponShootingType::Type WeaponShootingType = EWeaponShootingType::Unknown;
	TSoftObjectPtr<AWeaponBase> Weapon = nullptr;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnChangeShootingStateDelegate OnChangeShootingStateDelegate;

protected:
	void StopShootingViaReloading();
	ANpcAiController* GetNpcAiController() const;
	ANpcAiCharacter* GetNpcAiCharacter() const;
	
private:
	FTimerHandle ShootingTimerHandle;
	EShootingState ShootingState;

	void SetupShooting();
	void StartShootingImplementation();
	void StopShootingImplementation();

	void ShootSingleCallback();
	void ShootAutoCallback();
	UFUNCTION()
	void OnReloadingFinishedCallback(AWeaponBase* WeaponBase);
};
