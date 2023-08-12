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
	UFUNCTION(BlueprintCallable, Category = "PointOfInterest")
		UPARAM(DisplayName = "Does Index Exists?")bool  SetCurrentPointOfInterest(int32 Index);
	UFUNCTION(BlueprintPure, Category = "PointOfInterest")
		AAiPointOfInterest* GetCurrentPointOfInterest() const;
	template<class T>
		T* GetCurrentPointOfInterest() const;
	
	UFUNCTION(BlueprintCallable, Category = "PointOfInterest")
		void SwapToNextInterest();

protected:
	int32 IndexOfCurrentPointOfInterest;
	AAiPointOfInterest* CurrentPoi = nullptr;
	
private:
	AVillagerCharacter* GetControlledAiCharacter() const;	
	void SpawnPoiFromInstance(UAiPointOfInterestInstance* Instance);

private:
	bool bIsAfterSetOfPoi = false;	
};







template <class T>
T* AVillagerAiController::GetCurrentPointOfInterest() const
{
	const auto Result = Cast<T>(GetCurrentPointOfInterest());
	return Result;
}
