// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Npc/NpcAiController.h"
#include "ANpcEnemyController.generated.h"

class UNpcPerceptionComponent;
class UAIPerceptionComponent;
struct FActorPerceptionUpdateInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnNoticeActorDelegate,
	AANpcEnemyController*, EnemyNpcController, 
	AActor*, NoticedActor,
	EHostileNoticeState, HostileNoticeState,
	AActor*, OldActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnNoticeStateUpdateDelegate,
	AANpcEnemyController*, EnemyNpcController, 
	AActor*, NoticedActor,
	EHostileNoticeState, HostileNoticeState);

UCLASS()
class ANDROIDTEST_API AANpcEnemyController : public ANpcAiController
{
	GENERATED_BODY()

public:
	AANpcEnemyController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UNpcPerceptionComponent* GetEnemyPerceptionComponent() const { return EnemyPerceptionComponent; }
	
	UFUNCTION(BlueprintPure, Category = "Ai")
		AActor* GetCurrentNoticedActor(EHostileNoticeState& OutHostileNoticeState) const;
	// Sets Stress Progress Too
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void NoticeActor(AActor* NewActor);

	EHostileNoticeState GetHostileNoticeState() const { return HostileNoticeState; }

	UFUNCTION(BlueprintPure, Category = "Ai")
		const AActor* GetLastNoticedActor(UPARAM(DisplayName = "OutDoesExists?") bool& OutDoesExists) const;

	// Sets Hostile Notice State
	// 0.f => None
	// 1.f => Chaise
	// else => Notice
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void SetChaiseStressProgress(float InChaiseStressProgress);
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void AddDeltaToChaiseStressProgress(float Delta);
	float GetChaiseStressProgress() const { return ChaiseStressProgress; }

	/*components:*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UNpcPerceptionComponent* EnemyPerceptionComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Options")
		float FocusTurnAngle = 45;
	
public: /*delegates:*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnNoticeActor")
		FOnNoticeActorDelegate OnNoticeActorDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnNoticeStateUpdate")
		FOnNoticeStateUpdateDelegate OnNoticeStateUpdateDelegate;
	
protected:
	TSoftObjectPtr<AActor> CurrentNoticingActor = nullptr;
	TSoftObjectPtr<AActor> LastNoticingActor = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Ai")
		EHostileNoticeState HostileNoticeState = EHostileNoticeState::None;
	// If stress == 1 then chaise starts
	// If stress == 0 then chaise stops
	UPROPERTY(BlueprintReadOnly, Category = "Ai", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float ChaiseStressProgress = 0.f;

protected:	/* Stress options: */
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float OnNoticedAdder = 0.1;

	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float DamageAdder = 1.f;

	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float SightAdderEverySecond = 0.4f;	
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float SightDiffEverySecond = 0.02f;

	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float HearingAdderEverySecond = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Ai|Stress", meta = (ClampMin = 0.f, ClampMax = 1.f))
		float HearingDiffEverySecond = 0.f;

protected:
	void SetHostileNoticeState(EHostileNoticeState InHostileNoticeState);
	
private:
	UFUNCTION()
		void ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo);
	UFUNCTION()
		void PawnDamageCallback(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
								class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
								const class UDamageType* DamageType, AActor* DamageCauser);
	void ChaiseStressUpdate(float DeltaTime);
};
