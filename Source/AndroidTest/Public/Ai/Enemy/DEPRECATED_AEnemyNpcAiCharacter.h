// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "DEPRECATED_AEnemyNpcAiCharacter.generated.h"

class UAIPerceptionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusedActorChanged,
	AActor*, NewFocusedActor);

UCLASS(Deprecated)
class ANDROIDTEST_API ADEPRECATED_AEnemyNpcAiCharacter : public ANpcAiCharacter
{
	GENERATED_BODY()

public:
	ADEPRECATED_AEnemyNpcAiCharacter();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	virtual FGenericTeamId GetGenericTeamId() const override;

	UFUNCTION(BlueprintPure)
		AActor* GetFocusedActor() const;
	UFUNCTION(BlueprintCallable)
		void SetFocusedActor(AActor* InFocusedActor);
	UFUNCTION(BlueprintPure)
		FVector GetLastPositionOfFocusedActor() const;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UAIPerceptionComponent* PerceptionComponent;
	UPROPERTY(BlueprintGetter = GetFocusedActor, BlueprintSetter = SetFocusedActor, Category = "Enemy")
		AActor* FocusedActor;
	FVector LastPositionOfFocusedActor = FVector::ZeroVector;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnFocusedActor")
		FOnFocusedActorChanged OnFocusedActorChanged;
	
protected:
	UFUNCTION()
		void ActorPerceptionUpdateCallBack(const FActorPerceptionUpdateInfo& Info);
};
