// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NpcAiController.generated.h"

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
	ANpcAiCharacter* GetControlledAiCharacter() const;	
	void SpawnPoiFromInstance(UAiPointOfInterestInstance* Instance);

private:
	bool bIsAfterSetOfPoi = false;	
};







template <class T>
T* ANpcAiController::GetCurrentPointOfInterest() const
{
	const auto Result = Cast<T>(GetCurrentPointOfInterest());
	return Result;
}
