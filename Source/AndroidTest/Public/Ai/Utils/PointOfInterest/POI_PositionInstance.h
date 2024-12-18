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
	Actor			UMETA(DisplayName = "Actor"),
	Route			UMETA(DisplayName = "Route")
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options",
		meta = (EditCondition = "PositionSource == EPOI_PositionInstancePositionSource::Route", EditConditionHides))
		TSoftObjectPtr<AActor> Route;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		float CollisionRadius = 300.f;
};

UCLASS()
class APOI_Position : public AAiPointOfInterest
{
private:
	GENERATED_BODY()

public:
	APOI_Position();
	
	virtual void Init(UAiPointOfInterestInstance* Instance, ANpcAiController* AiController) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Components)
		USphereComponent* SphereCollisionComponent;

protected:
	virtual void OnComplete() override;

private:
	void OnCompleteWrapper();
};
