// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Npc/NpcAiController.h"
#include "ANpcEnemyController.generated.h"

class UAIPerceptionComponent;
struct FActorPerceptionUpdateInfo;

UCLASS()
class ANDROIDTEST_API AANpcEnemyController : public ANpcAiController
{
	GENERATED_BODY()

public:
	AANpcEnemyController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UAIPerceptionComponent* GetEnemyPerceptionComponent() const { return EnemyPerceptionComponent; }
	
public: /*components:*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UAIPerceptionComponent* EnemyPerceptionComponent;

	UFUNCTION(BlueprintPure, Category = "Ai")
		AActor* GetCurrentChasingActor(UPARAM(DisplayName = "IsChasingActor?") bool& OutIsChasingActor) const;

private:
	void ChasingActorCheck(float DeltaTime);
	UFUNCTION()
		void ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo);

private:
	TSoftObjectPtr<AActor> CurrentChasingActor = nullptr;
};
