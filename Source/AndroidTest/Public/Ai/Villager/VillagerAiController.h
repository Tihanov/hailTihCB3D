// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VillagerAiController.generated.h"

class AVillagerCharacter;
class UAiPointOfInterestInstance;
class AAiPointOfInterest;

UCLASS()
class ANDROIDTEST_API AVillagerAiController : public AAIController
{
	GENERATED_BODY()

public:
	AVillagerAiController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	UFUNCTION(BlueprintPure)
		AAiPointOfInterest* GetCurrentPointOfInterest() const;
	UFUNCTION(BlueprintCallable)
		void SwapToNextInterest();
	

	template<class T>
	T* GetCurrentPointOfInterest() const;

protected:
	int32 IndexOfCurrentPointOfInterest;
	AAiPointOfInterest* CurrentPoi = nullptr;
	
private:
	AVillagerCharacter* GetControlledAiCharacter() const;	
	void SpawnPoiFromInstance(UAiPointOfInterestInstance* Instance);
};







template <class T>
T* AVillagerAiController::GetCurrentPointOfInterest() const
{
	const auto Result = Cast<T>(GetCurrentPointOfInterest());
	return Result;
}
