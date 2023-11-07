// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Npc/NpcAiController.h"
#include "NpcEnemyController.generated.h"

class UNpcPerceptionComponent;
class UAIPerceptionComponent;
struct FActorPerceptionUpdateInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetActorSetDelegate,
	ANpcEnemyController*, EnemyNpcController, 
	AActor*, TargetActor);

UCLASS()
class ANDROIDTEST_API ANpcEnemyController : public ANpcAiController
{
	GENERATED_BODY()

public:
	ANpcEnemyController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UNpcPerceptionComponent* GetEnemyPerceptionComponent() const { return EnemyPerceptionComponent; }
	
	UFUNCTION(BlueprintPure, Category = "Ai")
		AActor* GetTargetActor(UPARAM(DisplayName = "IsNull?") bool& IsNull) const;
	AActor* GetTargetActor() const { return TargetActor.Get(); };
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void SetTargetActor(AActor* NewActor);

	UFUNCTION(BlueprintPure, Category = "Ai")
		const AActor* GetLastTargetActor(UPARAM(DisplayName = "DoesExist?") bool& OutDoesExist) const;
	const AActor* GetLastTargetActor() const {return LastTargetActor.Get(); }

	// 0.f => None
	// 1.f => Aggressive
	// else => Notice
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void SetStressProgress(float InChaiseStressProgress);
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void AddDeltaToStressProgress(float Delta);
	float GetStressProgress() const { return StressProgress; }

	bool IsFreezeStressProgress() const { return bFreezeStressProgress; }
	void SetFreezeStressProgress(bool bInFreezeStressProgress) { bFreezeStressProgress = bInFreezeStressProgress; }

	/*components:*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UNpcPerceptionComponent* EnemyPerceptionComponent;

public: /*delegates:*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnTargetActorSet")
		FOnTargetActorSetDelegate OnTargetActorSetDelegate;
	
protected:
	TSoftObjectPtr<AActor> TargetActor = nullptr;
	TSoftObjectPtr<AActor> LastTargetActor = nullptr; // TODO DEPRECATED
	// If stress == 1 then chaise starts
	// If stress == 0 then chaise stops
	UPROPERTY(BlueprintReadOnly, Category = "Ai", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float StressProgress = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Ai")
		bool bFreezeStressProgress = false;

protected:	/* Stress options: */
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float OnNoticedAdder = 0.1;
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float DamageAdder = 1.f;

	UPROPERTY(EditAnywhere, Category = "Ai|Stress")
		bool DoInterpolationOnSightDependsOnDistance = true;
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float SightAdderEverySecondMax = 1.f;	
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f,
		EditCondition = "DoInterpolationOnSightDependsOnDistance", EditConditionHides))
		float SightAdderEverySecondMin = 0.1f;	

	UPROPERTY(EditAnywhere, Category = "Ai|Stress")
		bool DoInterpolationOnHearingDependsOnDistance = true;
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float HearingAdderEverySecondMax = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f,
		EditCondition = "DoInterpolationOnHearingDependsOnDistance", EditConditionHides))
		float HearingAdderEverySecondMin = 0.f;	

	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float NoPerceptionAdderEverySecond = -0.2f;

private:
	UFUNCTION()
		void ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo);
	UFUNCTION()
		void PawnDamageCallback(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
								class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
								const class UDamageType* DamageType, AActor* DamageCauser);
	void ChaiseStressUpdate(float DeltaTime);
};
