// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AiPointOfInterest.h"
#include "POI_PositionInstance.generated.h"

class USphereComponent;

UENUM()
enum class EPOI_PositionInstancePositionSource : uint8
{
	Location		UMETA(DisplayName = "Location"),
	Actor			UMETA(DisplayName = "Actor")
};

UCLASS()
class ANDROIDTEST_API UPOI_PositionInstance : public UAiPointOfInterestInstance
{
	GENERATED_BODY()
public:
	UPOI_PositionInstance();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		EPOI_PositionInstancePositionSource PositionSource = EPOI_PositionInstancePositionSource::Location;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options",
		meta = (EditCondition = "PositionSource == EPOI_PositionInstancePositionSource::Location", EditConditionHides))
		FVector Location;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options",
		meta = (EditCondition = "PositionSource == EPOI_PositionInstancePositionSource::Actor", EditConditionHides))
		TSoftObjectPtr<AActor> Actor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options",
		meta = (EditCondition = "PositionSource == EPOI_PositionInstancePositionSource::Actor", EditConditionHides))
		bool bAttachToActor = false;
	
};

UCLASS()
class APOI_Position : public AAiPointOfInterest
{
private:
	GENERATED_BODY()

public:
	APOI_Position();
	
	virtual void Init_Implementation(UAiPointOfInterestInstance* Instance, AVillagerAiController* AiController) override;
	virtual bool IsComplete_Implementation() const override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
		USphereComponent* SphereCollisionComponent;

protected:
	bool bIsComplete = false;
};
