// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeManager.generated.h"

USTRUCT(BlueprintType)
struct FClock
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0, ClampMax = 24)) int32 Hour;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0, ClampMax = 60)) int32 Minute;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0, ClampMax = 60)) int32 Second;
};

/**
 * 1 = A > B
 * 0 = A == B
 * -1 = A < B
 */
int32 ClockCompare(const FClock& A, const FClock& B);

UCLASS()
class ANDROIDTEST_API ATimeManager : public AActor
{
	GENERATED_BODY()

public:
	ATimeManager();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		float CountOfGameSecondsInOneRealSecond = 10.f;

	UFUNCTION(BlueprintCallable, Category = "Time")
		void SetCurrentTimeInSeconds(float Time);
	UFUNCTION(BlueprintPure, Category = "Time")
		float GetCurrentTimeInSeconds() const;

	UFUNCTION(BlueprintCallable, Category = "Time")
		void SetCurrentTime(FClock Clock);
	UFUNCTION(BlueprintPure, Category = "Time")
		FClock GetCurrentTime() const;

	// After this time CurrentTime resets
	UFUNCTION(BlueprintPure, Category = "Time")
		float GetMaxTime() const { return MAX_TIME; }
private:
	float CurrentTime = 0.f;
	const float MAX_TIME = 60.f * 60.f * 24.f;
};
