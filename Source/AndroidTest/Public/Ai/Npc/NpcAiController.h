// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ai/Utils/NpcUtils.h"
#include "NpcAiController.generated.h"

class UHealthPointsComponent;
class ANpcAiCharacter;
class UAiPointOfInterestInstance;
class AAiPointOfInterest;

UCLASS()
class ANDROIDTEST_API ANpcAiController : public AAIController
{
	GENERATED_BODY()

public:
	ANpcAiController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	UFUNCTION(BlueprintCallable, Category = "PointOfInterest")
		UPARAM(DisplayName = "Does Index Exists?")bool  SetCurrentPointOfInterest(int32 Index);
	UFUNCTION(BlueprintPure, Category = "PointOfInterest")
		AAiPointOfInterest* GetCurrentPointOfInterest() const;
	template<class T>
		T* GetCurrentPointOfInterest() const;
	
	UFUNCTION(BlueprintCallable, Category = "PointOfInterest")
		void SwapToNextInterest();

	UFUNCTION(BlueprintSetter, BlueprintCallable)
		void SetNpcTeamId(TEnumAsByte<ENpcTeamId> NewNpcTeamId);
	UFUNCTION(BlueprintGetter, BlueprintPure)
		TEnumAsByte<ENpcTeamId> GetNpcTeamId() const;

	/*
	 * Implementation of IGenericTeamAgentInterface
	 */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		TEnumAsByte<ENpcTeamId> DefaultNpcTeamId = NTI_Villager;
	
	int32 IndexOfCurrentPointOfInterest;
	AAiPointOfInterest* CurrentPoi = nullptr;
	
private:
	ANpcAiCharacter* GetControlledAiCharacter() const;	
	void SpawnPoiFromInstance(UAiPointOfInterestInstance* Instance);
	void RerunBehavior();
	UFUNCTION()
		void OnNpcStateChangeCallback(ENpcState NewState, ENpcState OldState);
	
private:
	bool bIsAfterSetOfPoi = false;

private:
	UFUNCTION()
		void OnPawnDeathCallback(UHealthPointsComponent* HpComponent);
};







template <class T>
T* ANpcAiController::GetCurrentPointOfInterest() const
{
	const auto Result = Cast<T>(GetCurrentPointOfInterest());
	return Result;
}
