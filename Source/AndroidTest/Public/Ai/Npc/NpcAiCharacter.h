// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Ai/Utils/NpcUtils.h"
#include "Weapon/WeaponBase.h"
#include "Npc/NpcBaseCharacter.h"
#include "NpcAiCharacter.generated.h"

class UNpcMovementComponent;
class UTeamIdComponent;
class UChestComponent;
class UHealthPointsComponent;
class UAIPerceptionStimuliSourceComponent;
class UBehaviorTree;
class UAiPointOfInterestInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNpcChangeStateDelegate,
	ENpcState, NewState,
	ENpcState, OldState);

UCLASS(BlueprintType, Blueprintable)
class ANDROIDTEST_API ANpcAiCharacter
	: public ANpcBaseCharacter
{
	GENERATED_BODY()

public:
	ANpcAiCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual FVector GetPawnViewLocation() const override;
	
public:
	template<class T = UHealthPointsComponent> UHealthPointsComponent* GetHpComponent() const { return Cast<T>(HpComponent); }
	
	bool IsHaveWeapon() const { return bHaveWeapon; }
	FName GetWeaponItemName() const { return WeaponItemName; }
	FName GetWeaponSocket() const { return WeaponSocket; }
	bool IsEquipWeapon() const { return EquippedWeapon.IsValid(); }
	TSoftObjectPtr<AWeaponBase> GetEquippedWeapon() const { return EquippedWeapon; }
	UFUNCTION(BlueprintCallable, Category = "Ai|Weapon")
		void EquipWeapon(FName SocketName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "Ai|Weapon")
		void UnequipWeapon();

	// -1.f take interval from DataTable
	UFUNCTION(BlueprintCallable, Category = "Ai|Weapon")
		void StartShooting(float Interval = -1.f);
	UFUNCTION(BlueprintCallable, Category = "Ai|Weapon")
		void StopShooting();
	
	ENpcState GetCurrentState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void SetCurrentState(const ENpcState NewState);

	UBehaviorTree* GetBehaviorByState(const ENpcState State) const;
	UFUNCTION(BlueprintPure, Category = "Ai")
		UBehaviorTree* GetCurrentBehavior() const;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UHealthPointsComponent* HpComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UChestComponent* ChestComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UTeamIdComponent* TeamIdComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Ai|Point Of Interest")
		TArray<UAiPointOfInterestInstance*> PointsOfInterest;

public: /*delegates:*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnNpcChangeState")
		FOnNpcChangeStateDelegate OnNpcChangeStateDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|Sight")
		FName HeadSocketName = NAME_None;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|Weapon", DisplayName = "Have Weapon?")
		bool bHaveWeapon = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|Weapon", meta=(EditCondition = "bHaveWeapon", EditConditionHides))
		FName WeaponItemName = TEXT("None");
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|Weapon", meta=(EditCondition = "bHaveWeapon", EditConditionHides))
		FName WeaponSocket = NAME_None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|Weapon", meta=(EditCondition = "bHaveWeapon", EditConditionHides))
		UAnimMontage* AimWithWeaponAnimation = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|Weapon|Options", meta=(EditCondition = "bHaveWeapon", EditConditionHides))
		bool bDontShootIfNotReachable = true;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|State")
		ENpcState CurrentState = ENpcState::Patrol;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ai|State")
		TMap<ENpcState, UBehaviorTree*> StateBehavior;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ai|State", meta = (Bitmask, BitmaskEnum = ENpcState))
		uint8 FaceFocusStates = uint8(ENpcState::Patrol | ENpcState::Aggressive);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai")
		float HealPoints = 100.f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ai|Weapon")
		TSoftObjectPtr<AWeaponBase> EquippedWeapon = nullptr;

private:
	FTimerHandle ShootTimerHandler;

private:
	void ShootTimeCallback();
};
