// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "TurnAsActorCallback.generated.h"

UCLASS()
class ANDROIDTEST_API UTurnAsActorCallback : public UPoiCallback
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(AAiPointOfInterest* PointOfInterest) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		TSoftObjectPtr<AActor> InActor;
	// Should be > 0.1
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		float RotationDelay = 0.2f;
	
private:
	FTimerHandle TurnAsActorTimerHandle;
	void TurnAsActorCallback();
	APawn* Pawn;
};
