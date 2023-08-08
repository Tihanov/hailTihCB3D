// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeManager.generated.h"

UCLASS()
class ANDROIDTEST_API ATimeManager : public AActor
{
	GENERATED_BODY()

public:
	ATimeManager();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		float CountOfGameSecondsInOneRealSecond = 10.f;

	// Set Current Time In Seconds
	UFUNCTION(BlueprintCallable, Category = "Time")
		void SetCurrentTime(float Time);
	// Get Current Time In Seconds
	UFUNCTION(BlueprintPure, Category = "Time")
		float GetCurrentTime() const;

	// After this time CurrentTime resets
	UFUNCTION(BlueprintPure, Category = "Time")
		float GetMaxTime() const { return MAX_TIME; }

private:
	float CurrentTime = 0.f;
	const float MAX_TIME = 60.f * 60.f * 24.f;
};
