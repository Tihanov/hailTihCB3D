// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "POI_PositionInstance.h"
#include "Ai/Utils/TimeManager.h"
#include "POI_ClockAndPositionInstance.generated.h"

UCLASS()
class ANDROIDTEST_API UPOI_ClockAndPositionInstance : public UPOI_PositionInstance
{
	GENERATED_BODY()

public:
	UPOI_ClockAndPositionInstance();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clock")
		FClock Begin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clock")
		FClock End;
};

UCLASS()
class APOI_ClockAndPosition : public APOI_Position
{
	GENERATED_BODY()

public:
	APOI_ClockAndPosition();
	
	virtual void Init(UAiPointOfInterestInstance* Instance, ANpcAiController* AiController) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnComplete() override;

protected:
	TSoftObjectPtr<ATimeManager> TimeManagerSoft;

private:
	ATimeManager* GetTimeManagerFromGameMode() const;
};
