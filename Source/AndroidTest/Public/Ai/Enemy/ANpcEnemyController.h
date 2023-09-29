// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Npc/NpcAiController.h"
#include "ANpcEnemyController.generated.h"

class UNpcPerceptionComponent;
class UAIPerceptionComponent;
struct FActorPerceptionUpdateInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChaseNewActorDelegate,
	AActor*, NewActor,
	AActor*, OldActor);

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
		AActor* GetCurrentChasingActor(UPARAM(DisplayName = "IsChasingActor?") bool& OutIsChasingActor) const;
	UFUNCTION(BlueprintCallable, Category = "Ai")
		void ChaiseActor(AActor* NewActor);

	UFUNCTION(BlueprintPure, Category = "Ai")
		const AActor* GetLastChasingActor(UPARAM(DisplayName = "OutDoesExists?") bool& OutDoesExists) const;

public: /*components:*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UNpcPerceptionComponent* EnemyPerceptionComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Options")
		float FocusTurnAngle = 45;
	
public: /*delegates:*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnChaseNewActor")
		FOnChaseNewActorDelegate OnChaseNewActorDelegate;
	
private:
	UFUNCTION()
		void ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo);
	UFUNCTION()
		void PawnDamageCallback(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
								class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
								const class UDamageType* DamageType, AActor* DamageCauser);
	
private:
	TSoftObjectPtr<AActor> CurrentChasingActor = nullptr;
	TSoftObjectPtr<AActor> LastChasingActor = nullptr;
};
